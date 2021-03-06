#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void integrator_leapfrog_part1(int n_particles, double x[][3], double v[][3], double half_time_step){
	for (int i=0;i<n_particles;i++){
		x[i][0]  += half_time_step * v[i][0];
		x[i][1]  += half_time_step * v[i][1];
		x[i][2]  += half_time_step * v[i][2];
	}
}
void integrator_leapfrog_part2(int n_particles, double x[][3], double v[][3], double a[][3], double time_step, double half_time_step){
	for (int i=0;i<n_particles;i++){
		v[i][0] += time_step * a[i][0];
		v[i][1] += time_step * a[i][1];
		v[i][2] += time_step * a[i][2];
		x[i][0]  += half_time_step * v[i][0];
		x[i][1]  += half_time_step * v[i][1];
		x[i][2]  += half_time_step * v[i][2];
	}
}

void gravity_calculate_acceleration(int n_particles, double m[], double x[][3], double a[][3]) {
    double G = 6.6742367e-11; // m^3.kg^-1.s^-2
    for (int i=0; i<n_particles; i++){
		a[i][0] = 0;
		a[i][1] = 0;
		a[i][2] = 0;
        for (int j=0; j<n_particles; j++){
            if (j == i) {
                continue;
            }
            double dx = x[i][0] - x[j][0];
            double dy = x[i][1] - x[j][1];
            double dz = x[i][2] - x[j][2];
            double r = sqrt(dx*dx + dy*dy + dz*dz);
            double prefact = -G/(r*r*r) * m[j];
            a[i][0] += prefact * dx;
            a[i][1] += prefact * dy;
            a[i][2] += prefact * dz;
        }
    }
}

int main(int argc, char* argv[]) {
    const int n_particles = 2;
    double time = 0;
    double time_step = 0.08;
    double half_time_step = 0.5*time_step;
    double time_limit = 365.25 * 1e6;
    double x[n_particles][3];
    double v[n_particles][3];
    double a[n_particles][3];
    double m[n_particles];

    for (int i=0; i<n_particles; i++) {
        m[i] = 0;
        x[i][0] = 0;
        x[i][1] = 0;
        x[i][2] = 0;
        v[i][0] = 0;
        v[i][1] = 0;
        v[i][2] = 0;
        a[i][0] = 0;
        a[i][1] = 0;
        a[i][2] = 0;
    }
    m[0] = 0.08; // M_SUN
    m[1] = 3.0e-6; // M_SUN
    x[1][0] = 0.0162; // AU
    x[1][1] = 6.57192058353e-15; // AU
    x[1][2] = 5.74968548652e-16; // AU
    v[1][0] = -1.48427302304e-14;
    v[1][1] = 0.0399408809121;
    v[1][2] = 0.00349437429104;

    while(time <= time_limit) {
        integrator_leapfrog_part1(n_particles, x, v, half_time_step);
        time += half_time_step;
        gravity_calculate_acceleration(n_particles, m, x, a);
        integrator_leapfrog_part2(n_particles, x, v, a, time_step, half_time_step);
        time += half_time_step;
    }
    printf("Position 1: %f %f %f | Position 2: %f %f %f\n", x[0][0], x[0][1], x[0][2], x[1][0], x[1][1], x[1][2]);
}

