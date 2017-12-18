#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Int16.h"
#include "iostream"
#include "fstream"


class KalmanFilter
{
	public:
		KalmanFilter();
		void SetPosAndVel(double x, double vx);
		void AccCallback(const std_msgs::Float64::ConstPtr& msg);

	private:
		ros::NodeHandle nh;
		ros::Publisher pubKalman;
		ros::Subscriber subAcc;
		ros::Time lastTime;
		float posAndVelX[2];
		int rowA;
		int colA;
		float A[2][2];
		float B[2];
		float Pk[2][2];
		float errorObs[2][2];
		float KG[2][2];
		float eye[2][2];
		float speed;
		float tempSpeed;
		float stopPos;
		int count;
		std::ofstream PosFile;
		std::ofstream SpeedFile;
};

KalmanFilter::KalmanFilter()
{
	SetPosAndVel(0.0,0.0);
	pubKalman = nh.advertise<nav_msgs::Odometry>("kalman_output", 1);
	lastTime = ros::Time::now();
	rowA = 2;
	A[0][0] = 1.0;
	A[1][0] = 0.0;
	A[1][1] = 1.0;
	Pk[0][0] = 20*20;
	Pk[0][1] = Pk[1][0] = 0.0;
	Pk[1][1] = 3*3;
	errorObs[0][0] = 50*50;
	errorObs[0][1] = 0;
	errorObs[1][0] = 0;
	errorObs[1][1] = 9;
	eye[0][0] = 1;
	eye[1][0] = 0;
	eye[0][1] = 0;
	eye[1][1] = 1;
	speed = 0.0;
	PosFile.open("Pos.txt");
	SpeedFile.open("Speed.txt");
	stopPos = 0.0;
	count = 1;
}

void KalmanFilter::SetPosAndVel(double x, double vx)
{
	posAndVelX[0] = x;
	posAndVelX[1] = vx;
}

/*
void KalmanFilter::MultiplyMatrix(float mat1, int size)
{
	for (int iii = 0; iii < size; iii++)
		std::cout << "Array" << iii << ": " << a[iii];
}
*/

void PrintMatrix(float mat[2][2])
{
	int row = 2;
	int col = 2;
	for (int iii = 0; iii < row; iii++)
	{
		for (int jjj = 0; jjj < col; jjj++)
		{
			std::cout << " " << mat[iii][jjj];
		}
		std::cout << std::endl;
	}
}

void KalmanFilter::AccCallback(const std_msgs::Float64::ConstPtr& msg)
{
	float dt = (ros::Time::now() - lastTime).toSec();
	lastTime = ros::Time::now();
	//std::cout << "Acc: " << msg->data << " | " << "DT: " << dt << std::endl;
  std::cout << msg->data << std::endl;
	speed = (speed + tempSpeed) / 2;
	speed = speed + msg->data * dt;
	SpeedFile << speed << std::endl;

	A[0][1] = dt;
	B[0] = (dt*dt)/2.0;
	B[1] = dt;
	
	
	float aX[rowA];
	aX[0] = 0.0;
	aX[1] = 0.0;
	
	for (int iii = 0; iii < rowA; iii++)
	{
		for (int jjj = 0; jjj < rowA; jjj++)
		{
			aX[iii] = aX[iii] + A[iii][jjj]*posAndVelX[jjj];
		}
	}

	//std::cout << "aX: " << aX[0] << " || " << aX[1] << std::endl;
	
	float bA[rowA];

	for (int iii = 0; iii < rowA; iii++)
	{
		posAndVelX[iii] = aX[iii] + B[iii]*msg->data;
	}

	/*
	std::cout << "posAndVelX ------------------- " << std::endl;
	std::cout << posAndVelX[0] << " " << posAndVelX[1] << std::endl;
	std::cout << "posAndVelX ------------------- " << std::endl;
	std::cout << std::endl;
	*/
	/* ProcessKovairens ==================================== */

	float tempPk[rowA][rowA];

	for (int iii = 0; iii < rowA; iii++)
	{
		for (int jjj = 0; jjj < rowA; jjj++)
		{
			for (int kkk = 0; kkk < rowA; kkk++)
			{
				tempPk[iii][jjj] += A[iii][kkk]*Pk[kkk][jjj];
			}
		}
	}

	float transA[rowA][rowA];

	Pk[0][0] = tempPk[0][0]+tempPk[0][1];
	Pk[1][0] = tempPk[0][1];
	Pk[0][1] = tempPk[0][1];
	Pk[1][1] = tempPk[1][1];
	/*
	std::cout << "Pk ------------------- " << std::endl;
	std::cout << Pk[0][0] << " " << Pk[0][1] << std::endl;
	std::cout << Pk[1][0] << " " << Pk[1][1] << std::endl;
	std::cout << "Pk ------------------- " << std::endl;
	std::cout << std::endl;
	*/
	//std::cout << "Pk2: " << std::endl;

	//PrintMatrix(&Pk);

	/*ProcessKoeff end ===============================*/

	/*KalmarGain begin =============================*/
	
	float errorPk[rowA][rowA];

	for (int iii = 0; iii < rowA; iii++)
	{
		for (int jjj = 0; jjj < rowA; jjj++)
		{
			errorPk[iii][jjj] = Pk[iii][jjj] + errorObs[iii][jjj];
		}
	}
	/*
	std::cout << "errorPk ------------------- " << std::endl;
	std::cout << errorPk[0][0] << " " << errorPk[0][1] << std::endl;
	std::cout << errorPk[1][0] << " " << errorPk[1][1] << std::endl;
	std::cout << "errorPk ------------------- " << std::endl;
	std::cout << std::endl;
	*/
	float tempKG[rowA][rowA];

	/* Inverse */

	float tempErrorPk[rowA][rowA];

	tempErrorPk[0][0] = errorPk[1][1];
	tempErrorPk[1][1] = errorPk[0][0];
	tempErrorPk[0][1] = -errorPk[0][1];
	tempErrorPk[1][0] = -errorPk[1][0];

	float detErrorPk = 1/(errorPk[0][0]*errorPk[1][1] - errorPk[0][1]*errorPk[1][0]);

	for (int iii = 0; iii < 2; iii++)
	{
		for (int jjj = 0; jjj < 2; jjj++)
		{
			tempErrorPk[iii][jjj] = tempErrorPk[iii][jjj]*detErrorPk;
		}
	}

	/* Inverse end */
	
	tempKG[0][0] = 0.0;
	tempKG[1][0] = 0.0;
	tempKG[0][1] = 0.0;
	tempKG[1][1] = 0.0;

	for (int iii = 0; iii < rowA; iii++)
	{
		for (int jjj = 0; jjj < rowA; jjj++)
		{
			for (int kkk = 0; kkk < rowA; kkk++)
			{
				tempKG[iii][jjj] += Pk[iii][kkk]*tempErrorPk[kkk][jjj];
			}
		}
	}

	KG[0][0] = tempKG[0][0];
	KG[1][0] = tempKG[1][0];
	KG[0][1] = tempKG[0][1];
	KG[1][1] = tempKG[1][1];

/*
	std::cout << "KG ------------------- " << std::endl;
	std::cout << KG[0][0] << " " << KG[0][1] << std::endl;
	std::cout << KG[1][0] << " " << KG[1][1] << std::endl;
	std::cout << "KG ------------------- " << std::endl;
	std::cout << std::endl;
*/	
	/* KalmarGain end ===============================*/

	/* Prediction & Mesurement combo =============================*/

	tempSpeed = speed;
	if (speed <= 0.0)
	{
		speed = 0.0;
		posAndVelX[0] = stopPos;
	}
	
	float errorPosAndVel[2];
	errorPosAndVel[0] = posAndVelX[0] - posAndVelX[0];
	errorPosAndVel[1] = speed - posAndVelX[1];
	float tempPosAndVel = 0.0;
		
	for (int iii = 0; iii < rowA; iii++)
	{
		for (int jjj = 0; jjj < rowA; jjj++)
		{
			tempPosAndVel += KG[iii][jjj] * errorPosAndVel[jjj];
		}
		posAndVelX[iii] += tempPosAndVel;
		tempPosAndVel = 0.0;
	}
	
	stopPos = posAndVelX[0];



  std::cout	<< posAndVelX[0] << " " << posAndVelX[1] << std::endl;
	PosFile	<< posAndVelX[0] << " " << posAndVelX[1] << std::endl;


	/* Pre & Mes combo =====================*/

	/* PK update ============================*/
	
	float errorKG[2][2];
	for (int iii = 0; iii < rowA; iii++)
	{
		for (int jjj = 0; jjj < rowA; jjj++)
		{
			errorKG[iii][jjj] = eye[iii][jjj] - KG[iii][jjj];
		}
	}
	
	tempPk[0][0] = 0.0;
	tempPk[0][1] = 0.0;
	tempPk[1][0] = 0.0;
	tempPk[1][1] = 0.0;

	for (int iii = 0; iii < rowA; iii++)
	{
		for (int jjj = 0; jjj < rowA; jjj++)
		{
			for (int kkk = 0; kkk < rowA; kkk++)
			{
				tempPk[iii][jjj] += errorKG[iii][kkk]*Pk[kkk][jjj];
			}
		}
	}
	
	Pk[0][0] = tempPk[0][0];
	Pk[0][1] = tempPk[0][1];
	Pk[1][0] = tempPk[1][0];
	Pk[1][1] = tempPk[1][1];

	std::cout << "Count: " << count++ << std::endl;
/*
	std::cout << "Pk Update ------------------- " << std::endl;
	std::cout << Pk[0][0] << " " << Pk[0][1] << std::endl;
	std::cout << Pk[1][0] << " " << Pk[1][1] << std::endl;
	std::cout << "Pk Update ------------------- " << std::endl;
	std::cout << std::endl;
	std::cout << " ==================================== " << std::endl;
	*/
	/* PK update end =========================*/

	//std::cout << "bA: " << bA[0] << " || " << bA[1] << std::endl;
	//std::cout << "Pos and Vel: " << posAndVelX[0] << " || " << posAndVelX[1] << std::endl;
	
}

void QuitCallback(const std_msgs::Int16::ConstPtr &msg)
{
	std::cout << "Done!" << std::endl;
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "kalman_node");
	ros::NodeHandle n;
	ROS_INFO("KalmanFilter");

	KalmanFilter kalmanFilter;
	ros::Subscriber subAcc = n.subscribe("AccX", 1, &KalmanFilter::AccCallback, &kalmanFilter);
	ros::Subscriber subQuit = n.subscribe("quit", 1, QuitCallback);
	
	while(ros::ok())
	{
		ros::spin();
	}
}


















