#include <pigpio.h>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

#include "PCA9685.h"

static constexpr int Motor_A = 17;
static constexpr int Motor_B = 27;
static constexpr int PWM_A = 4;
static constexpr int PWM_B = 5;

// Made with ChatGPT

class TB6612_Motor {
public:
    TB6612_Motor(int motor_pin, int pwm_pin) : motor_pin_(motor_pin), pwm_pin_(pwm_pin), offset_(1) {
        gpioSetMode(this->motor_pin_, PI_OUTPUT);
        gpioSetMode(this->pwm_pin_, PI_OUTPUT);
        gpioWrite(this->motor_pin_, 0);
        gpioPWM(this->pwm_pin_, 0);
    }

    void setPWM(int value) {
        gpioPWM(this->pwm_pin_, value);
    }

    void forward() {
        gpioWrite(this->motor_pin_, this->offset_);
    }

    void backward() {
        gpioWrite(this->motor_pin_, 1 - this->offset_);
    }

    void stop() {
        gpioWrite(this->motor_pin_, 0);
        gpioPWM(this->pwm_pin_, 0);
    }

    void setOffset(int offset) {
        this->offset_ = offset;
    }

private:
    int motor_pin_;
    int pwm_pin_;
    int offset_;
};

class Back_Wheels {
public:
    Back_Wheels(int bus_number = 1) : left_wheel_(std::make_unique<TB6612_Motor>(Motor_A, PWM_A)),
                                      right_wheel_(std::make_unique<TB6612_Motor>(Motor_B, PWM_B)) {
        this->forward_A_ = true;
        this->forward_B_ = true;

        std::cout << "Initializing PCA9685" << std::endl;
        this->pca9685_.init(bus_number, 0x40);

        this->speed_ = 0;
    }

    void forward() {
        this->left_wheel_->forward();
        this->right_wheel_->forward();
        std::cout << "Forward" << std::endl;
    }

    void backward() {
        this->left_wheel_->backward();
        this->right_wheel_->backward();
        std::cout << "Backward" << std::endl;
    }

    void stop() {
        this->left_wheel_->stop();
        this->right_wheel_->stop();
        std::cout << "Stop" << std::endl;
    }

    int getSpeed() const {
        return this->speed_;
    }

    void setSpeed(int speed) {
        this->speed_ = speed;
        this->pca9685_.setPWM(PWM_A, 0, speed * 40);
        this->pca9685_.setPWM(PWM_B, 0, speed * 40);
        std::cout << "Set speed to " << this->speed_ << std::endl;
    }

    void ready() {
        this->left_wheel_->setOffset(this->forward_A_);
        this->right_wheel_->setOffset(this->forward_B_);
        this->stop();
        std::cout << "Ready" << std::endl;
    }

    void calibration() {
        this->setSpeed(50);
        this->forward();
        this->cali_forward_A_ = this->forward_A_;
        this->cali_forward_B_ = this->forward_B_;
        std::cout << "Calibration" << std::endl;
    }

    void caliLeft() {
        this->cali_forward_A_ = (1 + this->cali_forward_A_) & 1;
        this->left_wheel_->setOffset(this->cali_forward_A_);
        this->forward();
        std::cout << "CaliLeft" << std::endl;
    }

    void caliRight() {
        this->cali_forward_B_ = (1 + this->cali_forward_B_) & 1;
        this->right_wheel_->setOffset(this->cali_forward_B_);
        this->forward();
        std::cout << "CaliRight" << std::endl;
    }

    void caliOK() {
        this->forward_A_ = this->cali_forward_A_;
        this->forward_B_ = this->cali_forward_B_;
        this->stop();
        std::cout << "CaliOK" << std::endl;
    }

private:
    std::unique_ptr<TB6612_Motor> left_wheel_;
    std::unique_ptr<TB6612_Motor> right_wheel_;
    PCA9685 pca9685_;
    int forward_A_;
    int forward_B_;
    int cali_forward_A_;
    int cali_forward_B_;
    int speed_;
};

void test() {
    std::cout << "Initializing GPIO" << std::endl;
    gpioInitialise();
    Back_Wheels back_wheels;
    const float DELAY = 0.01f;
    try {
        back_wheels.forward();
        for (int i = 0; i <= 100; ++i) {
            back_wheels.setSpeed(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(DELAY * 1000)));
        }
        for (int i = 100; i >= 0; --i) {
            back_wheels.setSpeed(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(DELAY * 1000)));
        }

        back_wheels.backward();
        for (int i = 0; i <= 100; ++i) {
            back_wheels.setSpeed(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(DELAY * 1000)));
        }
        for (int i = 100; i >= 0; --i) {
            back_wheels.setSpeed(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(DELAY * 1000)));
        }
    } catch (...) {
        back_wheels.stop();
    }
    std::cout << "Terminating GPIO" << std::endl;
    gpioTerminate();
}

int main() {
    test();
    return 0;
}
