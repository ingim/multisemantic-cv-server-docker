#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <vector>

#include "crow.h"

#include <opencv2/core/core.hpp>

#include "System.h"
// #include "Converter.h"

using namespace std;
using namespace cv;

class ImageGrabber {
public:
    ImageGrabber(ORB_SLAM3::System* pSLAM): mpSLAM(pSLAM) {}
    void GrabImage();
    ORB_SLAM3::System* mpSLAM;
};

int main(int argc, char **argv) {
    // if (argc != 4) {
    //     cerr << endl << "Usage: ./mscv_slam_dev_service path_to_vocabulary path_to_settings username" << endl;        
    //     return 1;
    // }

    // // Create SLAM system. It initializes all system threads and gets ready to process frames.
    // ORB_SLAM3::System SLAM(argv[1], argv[2], ORB_SLAM3::System::MONOCULAR, true);

    crow::SimpleApp app;
    CROW_ROUTE(app, "/slam_1").methods("POST"_method)
    ([](const crow::request& req){
        auto data = crow::json::load(req.body);
        if (!data)
            return crow::response(crow::status::BAD_REQUEST); // same as crow::response(400)
        else {
            auto w = data["width"].i();
            auto h = data["height"].i();
            cout << "Width: " << w << ", height: " << h << endl;

            Mat mat_img(h, w, CV_8UC3);
            crow::json::rvalue* rv_img = data["img"].begin();
            unsigned char *ptr = mat_img.data;
            for (auto& x : *rv_img) {
                *ptr = (unsigned char)x.i();
                ++ptr;
            }
        }

        crow::json::wvalue x({{}});
        x["output"] = "slam results";
        return crow::response(x);
    });
    app.port(50005).run();

    // Stop all threads
    // SLAM.Shutdown();
    return 0;
}

void ImageGrabber::GrabImage() {
    // Copy the ros image message to cv::Mat.
    // cv_bridge::CvImageConstPtr cv_ptr;
    // try {
    //     cv_ptr = cv_bridge::toCvShare(msg);
    // } catch (cv_bridge::Exception& e) {
    //     ROS_ERROR("cv_bridge exception: %s", e.what());
    //     return;
    // }

    // float* s = mpSLAM->TrackMonocular(cv_ptr->image, cv_ptr->header.stamp.toSec()).data();
    // for (int i = 0; i < 4; i++) {
    //     cout << s[i] << ", ";
    // }
    // cout << endl;

    // geometry_msgs::Quaternion q_msg;
    // q_msg.x = s[0];
    // q_msg.y = s[1];
    // q_msg.z = s[2];
    // q_msg.w = s[3];
    // orb_pub->publish(q_msg);
}