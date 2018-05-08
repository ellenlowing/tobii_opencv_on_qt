#include "GestureClassifier.h"
#include <QMetaObject>
using namespace cv;
using namespace std;
GestureClassifier::GestureClassifier(QObject *parent) : QObject(parent), rect(parent->findChild<QObject*>("rect"))
{
    capture.open(0);
    //set height and width of capture frame
    capture.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);


    if (!face_cascade.load(face_cascade_name)){ printf("--(!)Error loading face cascade\n"); }
    if (!nose_cascade.load(nose_cascade_name)){ printf("--(!)Error loading nose cascade\n"); }

    for (int i = 0; i < 8; i++){
        Ypts.push_back(0);
        Xpts.push_back(0);
    }

    namedWindow(imageWindow);
}

GestureClassifier::~GestureClassifier()
{

}

void GestureClassifier::closeEvent(QCloseEvent *event)
{
    breakLoop=true;
    destroyAllWindows();
    capture.~VideoCapture();
    //this->destroy();
    event->accept();
}


void GestureClassifier::StartTracking(){
    while (true){

        if(breakLoop==true){
            return;
        }
        capture.read(frame);
        //mySkinDetect(frame, frameDest);
        if(initialized) {
            prevLoc = track(frame, prevLoc, tmpl, fullTmpl);
            //frameCount++;
        } else {
            //frameCount = 0;
            detectAndDisplay(frame, face_cascade, nose_cascade, searchCenter, searchSize);
            initialized = setTrackPoint(frame, searchCenter, prevLoc, tmpl, fullTmpl);
        }

        if(fixationInitialized){
            StartDetectingNodAndShake();
        }
        fixationTime = clock();
        fixationTicks = (double)(fixationTime - fixationTime0);
        if(fixationTicks > 4*CLOCKS_PER_SEC){
            fixationInitialized = false;
        }

        if(fixationInitialized) {
            circle(frame, prevLoc, 10, Scalar(255, 0, 0), 4, 8, 0);
        }else{
            circle(frame, prevLoc, 10, Scalar(255, 255, 255), 4, 8, 0);
        }
        imshow(imageWindow, frame);

        if(waitKey(1) == 27){
            breakLoop=true;
            destroyAllWindows();
            capture.~VideoCapture();
            return;
        }
    }
}

void GestureClassifier::StartDetectingNodAndShake(){
    Ypts.erase(Ypts.begin());
    Ypts.push_back(prevLoc.y);
    Xpts.erase(Xpts.begin());
    Xpts.push_back(prevLoc.x);

    bool shakeDetected = detectShake(Xpts);
    bool nodDetected = detectNod(Ypts);
    if(shakeDetected && !nodDetected){
        if(!shakeInitialized){
            emit shakeSignal();
            shakeInitialized = true;
            shakeTime0 = clock();
        }
    }
    if(nodDetected && !shakeDetected){
        if(!nodInitialized){
            emit nodSignal();
            nodInitialized = true;
            nodTime0 = clock();
        }
    }

    now = clock();
    shakeTicks = (double)(now-shakeTime0);
    nodTicks = (double)(now-nodTime0);

    if(nodTicks >2*CLOCKS_PER_SEC && nodInitialized){
        nodInitialized = false;
    }

    if(shakeTicks > 2*CLOCKS_PER_SEC && shakeInitialized){
        shakeInitialized = false;
    }
}

void GestureClassifier::detectAndDisplay(cv::Mat &frame, cv::CascadeClassifier face_cascade, cv::CascadeClassifier nose_cascade, Point &searchCenter, Size &searchSize)
{
    std::vector<cv::Rect> faces;
    Mat frame_gray;
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 3, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
    /*
    scaleFactor Parameter specifies how much the image size is reduced at each image scale.
    Basically the scale factor is used to create your scale pyramid. Your model has a fixed size defined during training, which is visible in the xml.
    This means that this size of face is detected in the image if present. However, by rescaling the input image, you can resize a larger face to a smaller one, making it detectable by the algorithm.
    1.05 is a good possible value for this, which means you use a small step for resizing, i.e.reduce size by 5 % , you increase the chance of a matching size with the model for detection is found.
    This also means that the algorithm works slower since it is more thorough. You may increase it to as much as 1.4 for faster detection, with the risk of missing some faces altogether.

    minNeighbors Parameter specifies how many neighbors each candidate rectangle should have to retain it. This parameter will affect the quality of the detected faces. Higher value results in less detections but with higher quality. 3~6 is a good value for it.

    minSize Parameter gives the minimum possible object size. Objects smaller than that are ignored.
    This parameter determines how small size you want to detect. You decide it. Usually, [30, 30] is a good start for face detection.

    You can also add maxSize Parameter at the end. Maximum possible object size.Objects bigger than this are ignored.
    This parameter determines how big size you want to detect. Again, you decide it. Usually, you don't need to set it manually, the default value assumes you want to detect without an upper limit on the size of the face.
    */


    for (size_t i = 0; i < faces.size(); i++)
    {
        //cv::Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
        //ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

        cv::Mat faceROI = frame_gray(faces[i]);
        vector<Rect> nose;
        nose_cascade.detectMultiScale(faceROI, nose, 1.1, 3, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
        if(nose.size() > 0){
            Point nose_left(faces[i].x + nose[0].x, faces[i].y + nose[0].y );
            Point nose_right(faces[i].x + nose[0].x + nose[0].width, faces[i].y + nose[0].y + nose[0].height);

            searchCenter = Point((nose_left.x + nose_right.x)/2, (nose_left.y + nose_right.y)/2 );
            searchSize = Size(nose[0].width, nose[0].height);

        }

    }

}

bool GestureClassifier::setTrackPoint(cv::Mat &frame, Point point, Point & prevLoc, Mat &templ, Mat &fullTempl)
{
    point = Point(point.x - 6, point.y+3);
    double templateSizeRatio = 0.08;
    Size fullImageSize = frame.size();
    Size fullTemplateSize = Point(frame.size().width * templateSizeRatio, frame.size().width*templateSizeRatio);
    if (point.x < 0 || point.x >= fullImageSize.width ||
        point.y < 0 || point.y >= fullImageSize.height)
    {
        return 0;
    }

    cv::Mat smallFrame = workingFrame(frame);
    Size imageSize = smallFrame.size();
    // scaleFactor is only set now
    int workingWidth = 640;
    float scaleFactor = (float) workingWidth/frame.size().width;
    Point scaledPoint = point * scaleFactor;

    Size templateSize = Point(workingWidth*templateSizeRatio, workingWidth*templateSizeRatio);

    // Get positions of the top-left corner of the region of interest (template) centered in (x,y) on the scaled image
    cv::Point roiOrigin = adjustPoint(Point((int)scaledPoint.x, (int)scaledPoint.y) - cv::Point(templateSize.width/2, templateSize.height/2), imageSize - templateSize);
    cv::Rect roi(roiOrigin.x, roiOrigin.y, templateSize.width, templateSize.height);
    templ = smallFrame(roi);

    // Get positions of the top-left corner of the region of interest (template) centered in (x,y) on the full image
    cv::Point fullRoiOrigin = adjustPoint(Point((int)point.x, (int)point.y) - cv::Point(fullTemplateSize.width/2, fullTemplateSize.height/2), fullImageSize - fullTemplateSize);
    cv::Rect fullRoi(fullRoiOrigin.x, fullRoiOrigin.y, fullTemplateSize.width, fullTemplateSize.height);
    fullTempl = frame(fullRoi);

    prevLoc = point;
    // circle(frame, prevLoc, 10, Scalar(255, 255, 255), 4, 8, 0);
    bool initialized = true;
    return initialized;
}

Point GestureClassifier::track(cv::Mat &frame, Point prevLoc, Mat templ, Mat fullTempl)
{
    // sanityCheck.checkInitialized();
    // sanityCheck.checkFrameNotEmpty(frame);
    // sanityCheck.checkFrameSize(frame);
    // Mat templ;
    // Mat fullTempl;

    cv::Mat smallFrame = workingFrame(frame);

    // Match template (only look in a region around the previous position)
    cv::Size searchSize(smallFrame.size().width / 3, smallFrame.size().height / 3);

    double templateSizeRatio = 0.08;
    int workingWidth = 640;
    float scaleFactor = (float) workingWidth/frame.size().width;
    Size imageSize = smallFrame.size();
    Size templateSize = Point(workingWidth*templateSizeRatio, workingWidth*templateSizeRatio);
    Size fullImageSize = frame.size();
    Size fullTemplateSize = Point(frame.size().width * templateSizeRatio, frame.size().width*templateSizeRatio);

    cv::Point matchLoc = match(smallFrame, templ, imageSize - templateSize, Point((int)prevLoc.x * scaleFactor, (int)prevLoc.y * scaleFactor), searchSize);
    // Update template for scaled image
    cv::Rect roi(matchLoc.x, matchLoc.y, templateSize.width, templateSize.height);
    templ = smallFrame(roi);

    // Match template for full image (only look a few pixels around the matched region
    cv::Point searchCenter((int) (matchLoc.x / scaleFactor + fullTemplateSize.width / 2),
                           (int) (matchLoc.y / scaleFactor + fullTemplateSize.height / 2));
    searchSize = cv::Size(fullTemplateSize.width + (int) 10 / scaleFactor,
                          fullTemplateSize.height + (int) 10 / scaleFactor);
    matchLoc = match(frame, fullTempl, fullImageSize - fullTemplateSize, searchCenter, searchSize);
    // Update template for full image
    cv::Rect fullRoi(matchLoc.x, matchLoc.y, fullTemplateSize.width, fullTemplateSize.height);
    fullTempl = frame(fullRoi);

    // Return center of matched region
    prevLoc = Point(matchLoc.x + fullTemplateSize.width/2, matchLoc.y + fullTemplateSize.height/2);
    return prevLoc;
}

Point GestureClassifier::match(cv::Mat &frame, cv::Mat &tmpl, cv::Size limits, cv::Point searchCenter, cv::Size searchSize)
{
    // Adjust search region
    int offX = (2 * searchCenter.x - searchSize.width) / 2; // Subtracting first may change the result
    int offY = (2 * searchCenter.y - searchSize.height) / 2; // Subtracting first may change the result
    if (offX < 0) offX = 0;
    if (offY < 0) offY = 0;
    int searchWidth = searchSize.width;
    int searchHeight = searchSize.height;
    if (offX + searchWidth > frame.size().width) searchWidth = frame.size().width - offX;
    if (offY + searchHeight > frame.size().height) searchHeight = frame.size().height - offY;
    if (searchWidth < tmpl.size().width || searchHeight < tmpl.size().height) // Search area is smaller than template
    {
        return searchCenter;
    }
    cv::Mat searchRegion(frame(cv::Rect(offX, offY, searchWidth, searchHeight)));

    int resultCols =  searchRegion.cols - tmpl.cols + 1;
    int resultRows = searchRegion.rows - tmpl.rows + 1;
    cv::Mat result(resultRows, resultCols, CV_32FC1);

    // Do the Matching and Normalize
    int match_method = CV_TM_SQDIFF;
    cv::matchTemplate(searchRegion, tmpl, result, match_method);
    cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    // Localizing the best match with minMaxLoc
    double minVal;
    double maxVal;
    cv::Point minLoc;
    cv::Point maxLoc;
    cv::Point matchLoc;

    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
    // For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    {
        matchLoc = minLoc;
    }
    else
    {
        matchLoc = maxLoc;
    }
    return adjustPoint(matchLoc + cv::Point(offX, offY), limits);
}

// Nod detected if more than half of the array, which contains consecutive position values for n frames,
// detects change in position of nose (within the preset range)
bool GestureClassifier::detectNod(vector<double> pts){
    double lower_threshold = 3;
    double higher_threshold = 30;
    int minCount = pts.size()/2;
    int count = 0;
    for (int i = 0; i < pts.size()-1; i++){
        if (abs(pts[i] - pts[i+1]) > lower_threshold && abs(pts[i] - pts[i+1]) < higher_threshold){
            count++;
        }
    }
    if(count >= minCount) {
        return true;
    }else{
        return false;
    }
}

bool GestureClassifier::detectShake(vector<double> pts){
    double lower_threshold = 3;
    double higher_threshold = 30;
    int minCount = pts.size()/2;
    int count = 0;
    for (int i = 0; i < pts.size() - 1; i++){
        if (abs(pts[i] - pts[i + 1]) > lower_threshold && abs(pts[i] - pts[i + 1]) < higher_threshold){
            count++;
        }
    }
    if(count >= minCount) {
        return true;
    }else{
        return false;
    }
}

void GestureClassifier::onNod(){
    cout << "SLOT: nod detected!" << endl;
    QMetaObject::invokeMethod(rect, "turnGreen");
}

void GestureClassifier::onShake(){
    cout << "SLOT: shake detected!" << endl;
    QMetaObject::invokeMethod(rect, "turnRed");
}

void GestureClassifier::onNewFixation(QPointF point){
    float x = point.x();
    float y = point.y();
    if(!fixationInitialized && x >= 800 && x <= 900 && y >= 450 && y <= 550){
        cout << "onNewFixation: " << x << ", " << y << endl;
        fixationTime0 = clock();
        fixationInitialized = true;
        //StartDetectingNodAndShake();
    }
}

cv::Mat GestureClassifier::workingFrame(cv::Mat &frame)
{
    cv::Mat resizedFrame;
    int workingWidth = 640;
    float scaleFactor = (float) workingWidth / frame.size().width;
    cv::resize(frame, resizedFrame, cv::Size(), scaleFactor, scaleFactor);
    return resizedFrame;
}

Point GestureClassifier::adjustPoint(cv::Point point, cv::Size limits)
{
    int x = adjustPosition(point.x, limits.width);
    int y = adjustPosition(point.y, limits.height);
    return cv::Point(x, y);
}

int GestureClassifier::adjustPosition(int pos, int limit)
{
    if (pos < 0)
    {
        return 0;
    }
    else if (pos >= limit)
    {
        return limit - 1;
    }
    return pos;
}

int GestureClassifier::myMax(int a, int b, int c) {
    int m = a;
    (void)((m < b) && (m = b));
    (void)((m < c) && (m = c));
    return m;
}

int GestureClassifier::myMin(int a, int b, int c) {
    int m = a;
    (void)((m > b) && (m = b));
    (void)((m > c) && (m = c));
    return m;
}

void GestureClassifier::mySkinDetect(Mat& src, Mat& dst) {
    //Surveys of skin color modeling and detection techniques:
    //Vezhnevets, Vladimir, Vassili Sazonov, and Alla Andreeva. "A survey on pixel-based skin color detection techniques." Proc. Graphicon. Vol. 3. 2003.
    //Kakumanu, Praveen, Sokratis Makrogiannis, and Nikolaos Bourbakis. "A survey of skin-color modeling and detection methods." Pattern recognition 40.3 (2007): 1106-1122.
    src.copyTo(dst);
    for (int i = 0; i < src.rows; i++){
        for (int j = 0; j < src.cols; j++){
            //For each pixel, compute the average intensity of the 3 color channels
            Vec3b intensity = src.at<Vec3b>(i, j); //Vec3b is a vector of 3 uchar (unsigned character)
            int B = intensity[0]; int G = intensity[1]; int R = intensity[2];
            if ((R > 95 && G > 40 && B > 20) && (myMax(R, G, B) - myMin(R, G, B) > 15) && (abs(R - G) > 15) && (R > G) && (R > B)){
                dst.at<Vec3b>(i, j) = intensity;
            }else{
                dst.at<Vec3b>(i,j) = Vec3b(0,0,0);
            }
        }
    }
}

