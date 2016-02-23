/*  L-BFGS-B is released under the "New BSD License" (aka "Modified BSD License" */
/*  or "3-clause license") */
/*  Please read attached file License.txt */

#include "lbfgsb.h"
/*
                            DRIVER 1 in Fortran 77 
    -------------------------------------------------------------- 
               SIMPLE DRIVER FOR L-BFGS-B (version 3.0) 
    -------------------------------------------------------------- 

       L-BFGS-B is a code for solving large nonlinear optimization 
            problems with simple bounds on the variables. 

       The code can also be used for unconstrained problems and is 
       as efficient for these problems as the earlier limited memory 
                         code L-BFGS. 

       This is the simplest driver in the package. It uses all the 
                   default settings of the code. 


    References: 

       [1] R. H. Byrd, P. Lu, J. Nocedal and C. Zhu, ``A limited 
       memory algorithm for bound constrained optimization'', 
       SIAM J. Scientific Computing 16 (1995), no. 5, pp. 1190--1208. 

       [2] C. Zhu, R.H. Byrd, P. Lu, J. Nocedal, ``L-BFGS-B: FORTRAN 
       Subroutines for Large Scale Bound Constrained Optimization'' 
       Tech. Report, NAM-11, EECS Department, Northwestern University, 
       1994. 


         (Postscript files of these papers are available via anonymous 
          ftp to eecs.nwu.edu in the directory pub/lbfgs/lbfgs_bcm.) 

                             *  *  * 

        March 2011   (latest revision) 
        Optimization Center at Northwestern University 
        Instituto Tecnologico Autonomo de Mexico 

        Jorge Nocedal and Jose Luis Morales, Remark on "Algorithm 778: 
        L-BFGS-B: Fortran Subroutines for Large-Scale Bound Constrained 
        Optimization"  (2011). To appear in  ACM Transactions on 
        Mathematical Software, 
    -------------------------------------------------------------- 
            DESCRIPTION OF THE VARIABLES IN L-BFGS-B 
    -------------------------------------------------------------- 

    n is an INTEGER variable that must be set by the user to the 
      number of variables.  It is not altered by the routine. 

    m is an INTEGER variable that must be set by the user to the 
      number of corrections used in the limited memory matrix. 
      It is not altered by the routine.  Values of m < 3  are 
      not recommended, and large values of m can result in excessive 
      computing time. The range  3 <= m <= 20 is recommended. 

    x is a DOUBLE PRECISION array of length n.  On initial entry 
      it must be set by the user to the values of the initial 
      estimate of the solution vector.  Upon successful exit, it 
      contains the values of the variables at the best point 
      found (usually an approximate solution). 

    l is a DOUBLE PRECISION array of length n that must be set by 
      the user to the values of the lower bounds on the variables. If 
      the i-th variable has no lower bound, l(i) need not be defined. 

    u is a DOUBLE PRECISION array of length n that must be set by 
      the user to the values of the upper bounds on the variables. If 
      the i-th variable has no upper bound, u(i) need not be defined. 

    nbd is an INTEGER array of dimension n that must be set by the 
      user to the type of bounds imposed on the variables: 
      nbd(i)=0 if x(i) is unbounded, 
             1 if x(i) has only a lower bound, 
             2 if x(i) has both lower and upper bounds, 
             3 if x(i) has only an upper bound. 

    f is a DOUBLE PRECISION variable.  If the routine setulb returns 
      with task(1:2)= 'FG', then f must be set by the user to 
      contain the value of the function at the point x. 

    g is a DOUBLE PRECISION array of length n.  If the routine setulb 
      returns with taskb(1:2)= 'FG', then g must be set by the user to 
      contain the components of the gradient at the point x. 

    factr is a DOUBLE PRECISION variable that must be set by the user. 
      It is a tolerance in the termination test for the algorithm. 
      The iteration will stop when 

       (f^k - f^{k+1})/max{|f^k|,|f^{k+1}|,1} <= factr*epsmch 

      where epsmch is the machine precision which is automatically 
      generated by the code. Typical values for factr on a computer 
      with 15 digits of accuracy in double precision are: 
      factr=1.d+12 for low accuracy; 
            1.d+7  for moderate accuracy; 
            1.d+1  for extremely high accuracy. 
      The user can suppress this termination test by setting factr=0. 

    pgtol is a double precision variable. 
      On entry pgtol >= 0 is specified by the user.  The iteration 
        will stop when 

                max{|proj g_i | i = 1, ..., n} <= pgtol 

        where pg_i is the ith component of the projected gradient. 
      The user can suppress this termination test by setting pgtol=0. 

    wa is a DOUBLE PRECISION  array of length 
      (2mmax + 5)nmax + 11mmax^2 + 8mmax used as workspace. 
      This array must not be altered by the user. 

    iwa is an INTEGER  array of length 3nmax used as 
      workspace. This array must not be altered by the user. 

    task is a CHARACTER string of length 60. 
      On first entry, it must be set to 'START'. 
      On a return with task(1:2)='FG', the user must evaluate the 
        function f and gradient g at the returned value of x. 
      On a return with task(1:5)='NEW_X', an iteration of the 
        algorithm has concluded, and f and g contain f(x) and g(x) 
        respectively.  The user can decide whether to continue or stop 
        the iteration. 
      When 
        task(1:4)='CONV', the termination test in L-BFGS-B has been 
          satisfied; 
        task(1:4)='ABNO', the routine has terminated abnormally 
          without being able to satisfy the termination conditions, 
          x contains the best approximation found, 
          f and g contain f(x) and g(x) respectively; 
        task(1:5)='ERROR', the routine has detected an error in the 
          input parameters; 
      On exit with task = 'CONV', 'ABNO' or 'ERROR', the variable task 
        contains additional information that the user can print. 
      This array should not be altered unless the user wants to 
         stop the run for some reason.  See driver2 or driver3 
         for a detailed explanation on how to stop the run 
         by assigning task(1:4)='STOP' in the driver. 

    iprint is an INTEGER variable that must be set by the user. 
      It controls the frequency and type of output generated: 
       iprint<0    no output is generated; 
       iprint=0    print only one line at the last iteration; 
       0<iprint<99 print also f and |proj g| every iprint iterations; 
       iprint=99   print details of every iteration except n-vectors; 
       iprint=100  print also the changes of active set and final x; 
       iprint>100  print details of every iteration including x and g; 
      When iprint > 0, the file iterate.dat will be created to 
                       summarize the iteration. 

    csave  is a CHARACTER working array of length 60. 

    lsave is a LOGICAL working array of dimension 4. 
      On exit with task = 'NEW_X', the following information is 
        available: 
      lsave(1) = .true.  the initial x did not satisfy the bounds; 
      lsave(2) = .true.  the problem contains bounds; 
      lsave(3) = .true.  each variable has upper and lower bounds. 

    isave is an INTEGER working array of dimension 44. 
      On exit with task = 'NEW_X', it contains information that 
      the user may want to access: 
        isave(30) = the current iteration number; 
        isave(34) = the total number of function and gradient 
                        evaluations; 
        isave(36) = the number of function value or gradient 
                                 evaluations in the current iteration; 
        isave(38) = the number of free variables in the current 
                        iteration; 
        isave(39) = the number of active constraints at the current 
                        iteration; 

        see the subroutine setulb.f for a description of other 
        information contained in isave 

    dsave is a DOUBLE PRECISION working array of dimension 29. 
      On exit with task = 'NEW_X', it contains information that 
        the user may want to access: 
        dsave(2) = the value of f at the previous iteration; 
        dsave(5) = the machine precision epsmch generated by the code; 
        dsave(13) = the infinity norm of the projected gradient; 

        see the subroutine setulb.f for a description of other 
        information contained in dsave 

    -------------------------------------------------------------- 
          END OF THE DESCRIPTION OF THE VARIABLES IN L-BFGS-B 
    -------------------------------------------------------------- 
    */



/* Main program */ 
//int MAIN__(void)
int main(void)
{

    /* System generated locals */
    integer i__1;
    double d__1, d__2;
    /* Local variables */
    static double f, g[1024];
    static integer i__;
    static double l[1024];
    static integer m, n;
    static double u[1024], x[1024], t1, t2, wa[43251];
    static integer nbd[1024], iwa[3072];
/*     static char task[60]; */
    static integer taskValue;
    static integer *task=&taskValue; /* must initialize !! */
/*      http://stackoverflow.com/a/11278093/269192 */
    static double factr;
/*     static char csave[60]; */
    static integer csaveValue;
    static integer *csave=&csaveValue;
    static double dsave[29];
    static integer isave[44];
    static logical lsave[4];
    static double pgtol;
    static integer iprint;


/*
    This simple driver demonstrates how to call the L-BFGS-B code to 
      solve a sample problem (the extended Rosenbrock function 
      subject to bounds on the variables). The dimension n of this 
      problem is variable. 
       nmax is the dimension of the largest problem to be solved. 
       mmax is the maximum number of limited memory corrections. 
    Declare the variables needed by the code. 
      A description of all these variables is given at the end of 
      the driver. 
    Declare a few additional variables for this sample problem. 
 */

/*     We wish to have output at every iteration. */
    iprint = 1; 
/*     iprint = 101; */
/*     We specify the tolerances in the stopping criteria. */
    factr = 1e7;
    pgtol = 1e-5;
/*     We specify the dimension n of the sample problem and the number */
/*        m of limited memory corrections stored.  (n and m should not */
/*        exceed the limits nmax and mmax respectively.) */
    n = 25;
    m = 5;
/*     We now provide nbd which defines the bounds on the variables: */
/*                    l   specifies the lower bounds, */
/*                    u   specifies the upper bounds. */
/*     First set bounds on the odd-numbered variables. */
    i__1 = n;
    for (i__ = 1; i__ <= i__1; i__ += 2) {
        l[i__ - 1] = 1.;
        u[i__ - 1] = 100.;
    }
    
    for(i__=1;i__<=i__1;i__++)
    {
        nbd[i__ - 1] = (i__<6)?1:((i__<11)?0:2);
    }
    /*     Next set bounds on the even-numbered variables. */
    i__1 = n;
    for (i__ = 2; i__ <= i__1; i__ += 2) {
        l[i__ - 1] = 100.;
        u[i__ - 1] = 100.;
    }
    /*     We now define the starting point. */
    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
        x[i__ - 1] = (i__<6)?-5:300.;
        /* L14: */
    }
    printf("     Solving sample problem (Rosenbrock test fcn).\n");
    printf("      (f = 0.0 at the optimal solution.)\n");

    
    
    printf("Current value of the pointer: %p\n", x); //Added by Tan for debugging
    /*     We start the iteration by initializing task. */

    int loopbreakcount = 0;
    
    *task = (integer)START;
/*     s_copy(task, "START", (ftnlen)60, (ftnlen)5); */
    /*        ------- the beginning of the loop ---------- */
L111://The code setup a looop here by using go to this L11
    /*     This is the call to the L-BFGS-B code. */
    
    //---Disable by Tan for debugging---
    setulb(&n, &m, x, l, u, nbd, &f, g, &factr, &pgtol, wa, iwa, task, &
            iprint, csave, lsave, isave, dsave);

    if ( IS_FG(*task) ) {//Disabled by Tan for Testing
    //---------------------------------
    
      /*        the minimization routine has returned to request the */
        /*        function f and gradient g values at the current x. */
        /*        Compute function value f for the sample problem. */
        /* Computing 2nd power */
        //f(x)=sum_over_(n_st_n>=0)([x(n)-x(n-1)^2]^2)+0.25*(x[0]-1)^2
        d__1 = x[0] - 1.;
        f = d__1 * d__1 * .25;
        i__1 = n;
        
        for (i__ = 2; i__ <= i__1; ++i__) {
            /* Computing 2nd power */
            d__2 = x[i__ - 2];
            /* Computing 2nd power */
            d__1 = x[i__ - 1] - d__2 * d__2;
            f += d__1 * d__1;
        }
        f *= 4.;
        //f(x)=4*sum_over_(n_st_n>=0)([x(n)-x(n-1)^2]^2)+(x[0]-1)^2
        
        //Added for debugging
        printf("Current value of the objective: %f\n",f);
        printf("Current value of the pointer (after setulb): %p\n", x);
        

        /*        Compute gradient g for the sample problem. */
        /* Computing 2nd power */
        d__1 = x[0];
        t1 = x[1] - d__1 * d__1;
        g[0] = (x[0] - 1.) * 2. - x[0] * 16. * t1;
        i__1 = n - 1;
        for (i__ = 2; i__ <= i__1; ++i__) {
            t2 = t1;
            /* Computing 2nd power */
            d__1 = x[i__ - 1];
            t1 = x[i__] - d__1 * d__1;
            g[i__ - 1] = t2 * 8. - x[i__ - 1] * 16. * t1;
            /* L22: */
        }
        g[n - 1] = t1 * 8.;
        
        
        
        //Added by Tan for debugging
        /*for (int printidx = 0;printidx<n;printidx++)
        {
            printf("[DEBUG] idx [%d]: x = %f, gfval = %f \n",(int)printidx,x[printidx],g[printidx]);
        }*/
        
        //Added by Tan for debugging
        loopbreakcount++;
        if (loopbreakcount<2)
            /*          go back to the minimization routine. */
            goto L111; //Disable by Tan for debugging
    }

/*     if (s_cmp(task, "NEW_X", (ftnlen)5, (ftnlen)5) == 0) { */
    if ( *task==NEW_X ) {
        goto L111;
    }
    /*        the minimization routine has returned with a new iterate, */
    /*         and we have opted to continue the iteration. */
    /*           ---------- the end of the loop ------------- */
    /*     If task is neither FG nor NEW_X we terminate execution. */
    //s_stop("", (ftnlen)0);
    return 0;
} /* MAIN__ */

///* Main program alias */ int driver_ () { MAIN__ (); return 0; }
