
#include <android/log.h>
#include <jni.h>

#include <lsd_slam/LiveSLAMWrapper.h>
#include <lsd_slam/IOWrapper/Timestamp.h>
#include <lsd_slam/util/Undistorter.h>

#define  LOG_TAG    "lsd-jni.cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static lsd_slam::LiveSLAMWrapper* slam;
static lsd_slam::UndistorterOpenCV* undistorter;


extern "C" {
JNIEXPORT void Java_de_joshuareibert_dontslamyourhead_MainActivity_initSLAM(
        JNIEnv* env, jobject thiz) {
    undistorter = new lsd_slam::UndistorterOpenCV(
            1100.66, 1100.66, 639.5, 359.5, 0.0737079, -0.0529861, 0, 0,
            1280, 720,
            "crop",
            320, 240);
    slam = new lsd_slam::LiveSLAMWrapper(undistorter);
}

JNIEXPORT void Java_de_joshuareibert_dontslamyourhead_MainActivity_updateSLAM(
        JNIEnv* env, jobject thiz,
        jlong grayImgAddress, jlong rgbaImgAddress, jlong depthImgAddress)
{
    cv::Mat& image = *(cv::Mat*)grayImgAddress;
    cv::Mat& out_image = *(cv::Mat*)rgbaImgAddress;
    cv::Mat& depth_image = *(cv::Mat*)depthImgAddress;
    cv::Mat undist_image;
    undistorter->undistort(image, undist_image);
    depth_image = slam->newImageCallback(undist_image, lsd_slam::Timestamp::now());
}

JNIEXPORT void Java_de_joshuareibert_dontslamyourhead_MainActivity_resetSLAM(
        JNIEnv* env, jobject thiz) {
    slam->resetAll();
}
}
