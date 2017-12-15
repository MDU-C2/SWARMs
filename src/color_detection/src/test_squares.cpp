// The "Square Detector" program.
// It loads several images sequentially and tries to find squares in
// each image
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <math.h>
#include <string.h>

using namespace cv;
using namespace std;

static void help()
{
    cout <<
    "\nA program using pyramid scaling, Canny, contours, contour simpification and\n"
    "memory storage (it's got it all folks) to find\n"
    "squares in a list of images pic1-6.png\n"
    "Returns sequence of squares detected on the image.\n"
    "the sequence is stored in the specified memory storage\n"
    "Call:\n"
    "./squares [file_name (optional)]\n"
    "Using OpenCV version " << CV_VERSION << "\n" << endl;
}


int thresh = 50, N = 11;
const char* wndname = "Square Detection Demo";
const char* crpd_wndname = "Found Squares";

int red_iLowH = 0;
int red_iHighH = 179;

int red_iLowS = 176; 
int red_iHighS = 255;

int red_iLowV = 47;
int red_iHighV = 255;


//////////////////////////

int green_iLowH = 25;//21;//51;//33;
int green_iHighH = 166;//119;//179;//99;

int green_iLowS = 85;//119;//175;//121; 
int green_iHighS = 255;

int green_iLowV = 0;
int green_iHighV = 76;//62;//105;//121;


// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
static double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
static void findSquares( const Mat& image, vector<vector<Point> >& squares )
{
    squares.clear();

    Mat pyr, timg, gray0(image.size(), CV_8U), gray;

    // down-scale and upscale the image to filter out the noise
    pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
    pyrUp(pyr, timg, image.size());
    vector<vector<Point> > contours;

    // find squares in every color plane of the image
    for( int c = 0; c < 3; c++ )
    {
        int ch[] = {c, 0};
        mixChannels(&timg, 1, &gray0, 1, ch, 1);

        // try several threshold levels
        //for( int l = 0; l < N; l++ )
        //{
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            //if( l == 0 )
            //{
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                Canny(gray0, gray, 0, thresh, 5);
                // dilate canny output to remove potential
                // holes between edge segments
                dilate(gray, gray, Mat(), Point(-1,-1));
            //}
            //else
            //{
            //    // apply threshold if l!=0:
            //    //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
            //    gray = gray0 >= (l+1)*255/N;
            //}

            // find contours and store them all as a list
            findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

            vector<Point> approx;

            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( approx.size() == 4 &&
                    fabs(contourArea(Mat(approx))) > 1000 &&
                    isContourConvex(Mat(approx)) )
                {
                    double maxCosine = 0;

                    for( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( maxCosine < 0.3 )
                        squares.push_back(approx);
                }
            }
        //}
    }
}


Point findColor( Mat &cropped, Rect bbox, int iLowH, int iLowS, int iLowV, int iHighH, int iHighS, int iHighV, vector<float> &line) //vector<Point> &line )
{
	Point center = Point(-1,-1);
	Mat imgHSV;

	cvtColor(cropped, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

	Mat imgThresholded;

	inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
  
	//morphological opening (removes small objects from the foreground)
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_RECT, Size(5, 5)) );
	dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_RECT, Size(5, 5)) ); 

	//morphological closing (removes small holes from the foreground)
	dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_RECT, Size(5, 5)) ); 
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_RECT, Size(5, 5)) );
	
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// THIS CHECK COULD BE USEFUL TO DISCARD FALSE POSITIVES, BUT IS NOT GOOD FOR NOW
	vector<vector<Point> > contour;
	findContours(imgThresholded, contour, RETR_LIST, CHAIN_APPROX_SIMPLE);
	if(contour.size() > 0) 
  {
    // previous implementation (Arman)
		//cout << "iLowH: " << iLowH << ", arcLength: " << arcLength(contour[0], false) << endl;
    fitLine(contour[0], line, CV_DIST_L2, 0, 0.01, 0.01);
    Moments m = moments(imgThresholded, false);
    center = Point( m.m10/m.m00 , m.m01/m.m00 );
    center.x += bbox.x;
    center.y += bbox.y;
	  cout << "iLowH: " << iLowH << ", Line_vX: " << line[0] << ", Line_vY: " << line[1] << endl;
    //cout << "--------------------" << endl;
    //cout << "iLowH: " << iLowH;
    //cout << "Line"
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  }

	//Moments m = moments(imgThresholded, false);
	//center = Point( m.m10/m.m00 , m.m01/m.m00 );

	//center.x += bbox.x;
	//center.y += bbox.y;
	
	//cout << "x: " <<  center.x << endl;
	//cout << "y: " <<  center.y << endl;

	//circle(imgThresholded, center, 2, Scalar(0, 0, 0));

	//imshow(crpd_wndname, imgThresholded);

	return center;
}

Point findGate(Mat cropped, Rect bbox)
{
  vector<float> redLine (4);
  vector<float> greenLine (4);
  Point centroid = Point(-1,-1);
  //Mat redLine;
  //Mat greenLine;
	Point red = findColor( cropped, bbox, red_iLowH, red_iLowS, red_iLowV, red_iHighH, red_iHighS, red_iHighV, redLine ); //red
	Point green = findColor( cropped, bbox, green_iLowH, green_iLowS, green_iLowV, green_iHighH, green_iHighS, green_iHighV, greenLine ); //green

	if(red.x != -1 && red.y != -1 && green.x != -1 && green.y != -1 &&
      abs(redLine[0] - greenLine[0]) < 0.1 && abs(redLine[1] - greenLine[1]) < 0.1)
	{
		centroid = Point( (red.x + green.x) / 2, (red.y + green.y) / 2 );
		cout << "Distance: " << sqrt( (red.x - green.x) * (red.x - green.x) + (red.y - green.y) * (red.y - green.y) ) << endl;
	}

	return centroid;	
}

// the function draws all the squares in the image
static void drawSquares( Mat& image, const vector<vector<Point> >& squares )
{
	Mat image_copy = image.clone();
    for( size_t i = 0; i < squares.size(); i++ )
    {
        const Point* p = &squares[i][0];
        int n = (int)squares[i].size();
		polylines(image, &p, &n, 1, true, Scalar(0,255,0), 3, LINE_AA);
        Rect bbox = boundingRect( squares[i] );
        if (bbox.x >= 30){
            bbox.x -= 30;
            bbox.width += 60;
        }
        if (bbox.y >= 30){
            bbox.y -= 30;
            bbox.height += 60;
        }
		//Check for the bounding box
		if (bbox.x >= 0 && bbox.width >= 0 && bbox.y >= 0 && bbox.height >= 0 && bbox.x + bbox.width <= image_copy.cols  && bbox.y + bbox.height <= image_copy.rows)
		{
			Mat cropped = image_copy(bbox);
			Point gate_centroid = findGate(cropped, bbox);
			if (gate_centroid.x != -1 && gate_centroid.y != -1)
			{
				circle(image, gate_centroid, 2, Scalar(255, 0, 0));
			}
		}
		rectangle(image, bbox.tl(), bbox.br(), Scalar(0, 0, 255), 2, 8, 0);
		
    }

    imshow(wndname, image);
}

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  vector<vector<Point> > squares;
	Mat image = cv_bridge::toCvShare(msg,"bgr8")->image;
	if(!image.empty())
	{
		findSquares(image, squares);
        drawSquares(image, squares);
		waitKey(30);
	}

}


int main(int argc, char** argv)
{
/*
    VideoCapture cap(0);
    if(!cap.isOpened())
    {
	  cout << "Cannot open the camera!" << endl; return 0;
    } 

    namedWindow( wndname, 1 );

	//namedWindow( crpd_wndname, 1);

    while(true)
    {
        Mat image;
        bool success = cap.read(image);
        if( !success )
        {
	      cout << "Cannot read the frame from the camera!" << endl; 
          break; 
        }
        
		findSquares(image, squares);
        drawSquares(image, squares);
        char c = (char)waitKey(5);
        if( c == 27 )
            break;
    }
*/

	ros::init(argc, argv, "test_squares");
	ros::NodeHandle nh;
	namedWindow( wndname );
	startWindowThread();
	image_transport::ImageTransport it(nh);
	image_transport::Subscriber sub = it.subscribe("/cv_camera/image_rect_color", 1, imageCallback);
	ros::spin();
	cv::destroyWindow(wndname);

    return 0;
}
