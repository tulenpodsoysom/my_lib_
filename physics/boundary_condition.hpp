#pragma once
// mostly satisfies this expression:  X_i = mu * X_i-1 + nu;
struct boundary_condition_first_order
{
    double mu{0};
    double nu{0};
	
	boundary_condition_first_order(double mu , double nu) : mu(mu), nu(nu){}

	template<typename T>
	T operator() (T Xi) {return evaluate_next(Xi);}

	template<typename T>
	T evaluate_next(T Xi) {return mu * Xi + nu;}
};




// mostly satisfies this expression:  k_1 * dfdx |x=a-0 = k_2 * dfdx |x=a+0;
// linspace is equal-spaced
struct boundary_condition_second_order
{
    double k1{1};
    double k2{1};
	boundary_condition_second_order(double k1 , double k2) : k1(k1), k2(k2){}

	template<typename T>
	T operator() (T left , T right) {return evaluate(left,right);}

	template<typename T>
	T evaluate(T left , T right) {double k = k2/k1; return (k*right + left)/(k+1);}
};
