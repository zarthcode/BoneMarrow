/** @file Filters.h
 *
 * Implementation of Complimentary and Kalman filters 
 *
 **/


/// Kalman Filter Instance
typedef struct
{

	float Q_angle;
	float Q_bias;
	float R_measure;

	float angle;
	float bias;
	float rate;

	float P[2][2];
	float K[2];
	float y;
	float S;

} KalmanFilter;
