
struct dimensionless_variables
{
    #define def_variable(a_name,a_normal_name,a_normal_value) \
    double a_normal_name = a_normal_value;\
    double a_name##_to_##a_normal_name(double a) {return a/a_normal_name;};\
    double a_normal_name##_to_##a_name(double a) {return a * a_normal_name;}
};
