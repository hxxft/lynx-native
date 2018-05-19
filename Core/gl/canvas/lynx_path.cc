#include "gl/canvas/lynx_path.h"

#include <math.h>
#include "base/print.h"
namespace canvas {

const float ERROR_DEVIATION = 1e-6;
const float PI_1 = M_PI;
const float PI_2 = 2.f * PI_1;

LxPath::LxPath()
    : path_stack_(),
      current_sub_path_(nullptr),
      begin_position_(),
      current_position_() {}

    LxPath::~LxPath(){}

void LxPath::PushPoint(float x, float y) {
  // if (current_position_.x_ == x && current_position_.y_ == y &&
  //     current_sub_path_.Get()) {
  //   return;
  // }
  if(!current_sub_path_ .Get()) {
    current_sub_path_.Reset(new SubPath); 
  }
  current_sub_path_->points_.push_back(LxPoint(x, y));
  current_position_.Set(x, y);
}

void LxPath::Clear() {
  path_stack_.clear();
  current_sub_path_.Reset();
  begin_position_.Set(0, 0);
  current_position_.Set(0, 0);
}

void LxPath::Stroke() {
  if (current_sub_path_.Get()) {
    path_stack_.push_back(current_sub_path_.Release()); 
  }
  current_sub_path_.Reset();   
}

void LxPath::MoveTo(float x, float y) {
  Stroke();
  begin_position_.Set(x, y);
  PushPoint(x, y);
}

void LxPath::LineTo(float x, float y) {
  PushPoint(x, y); 
}

void LxPath::Arc(float cx,
                 float cy,
                 float radius,
                 float start_angle,
                 float end_angle,
                 bool anti_clockwise) {
  float span_angle = end_angle - start_angle;
  if (anti_clockwise) {
    span_angle = -span_angle;
  }

  float span_angle_abs = fabs(span_angle);
  if (span_angle_abs < ERROR_DEVIATION) {  // the same angle,do nothing
    return;
  }

  if (span_angle_abs > 360) {
    end_angle = start_angle + 360;
    span_angle = 360;
  } else {
    while (span_angle < 0) {
      span_angle += 360 * 2;
    }
    while (span_angle > 360) {
      span_angle -= 360;
    }
  }

  int step_count = 5;
  step_count *= span_angle;
  if (step_count < 1) {
    step_count = 1;
  }
  float delta_angle = span_angle / step_count;
  if (anti_clockwise) {
    delta_angle = -delta_angle;
  }

  float dx = cosf(delta_angle/PI_2);
  float dy = sinf(delta_angle/PI_2);

  float offset_x = radius * cosf(start_angle/PI_2);
  float offset_y = radius * sinf(start_angle/PI_2);

  for (int step = 0; step <= step_count; ++step) {
    PushPoint(cx + offset_x, cy + offset_y);

    float old_x = offset_x, old_y = offset_y;
    offset_x = old_x * dx - old_y * dy;
    offset_y = old_x * dy + old_y * dx;
  }
}

void LxPath::ArcTo(float x1, float y1, float x2, float y2, float radius) {
  LxPoint cp = current_position_;

  float a1 = cp.y_ - y1;
  float b1 = cp.x_ - x1;
  float a2 = y2 - y1;
  float b2 = x2 - x1;
  float mm = fabsf(a1 * b2 - b1 * a2);

  if (mm < 1.0e-8 || radius == 0) {
    LineTo(x1, y1);
  } else {
    float dd = a1 * a1 + b1 * b1;
    float cc = a2 * a2 + b2 * b2;
    float tt = a1 * a2 + b1 * b2;
    float k1 = radius * sqrtf(dd) / mm;
    float k2 = radius * sqrtf(cc) / mm;
    float j1 = k1 * tt / dd;
    float j2 = k2 * tt / cc;
    float cx = k1 * b2 + k2 * b1;
    float cy = k1 * a2 + k2 * a1;
    float px = b1 * (k2 + j1);
    float py = a1 * (k2 + j1);
    float qx = b2 * (k1 + j2);
    float qy = a2 * (k1 + j2);
    float startAngle = atan2f(py - cy, px - cx);
    float endAngle = atan2f(qy - cy, qx - cx);

    Arc(cx + x1, cy + y1, radius, startAngle, endAngle, (b1 * a2 > b2 * a1));
  }
}
}