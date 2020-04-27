#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <math.h>

class complex {
    private:

    public:
        double real_part, imaginary_part;

        complex(double _real_part, double _imaginary_part) {
            real_part = _real_part, imaginary_part = _imaginary_part;
        }
        ~complex() {}

        complex operator + (complex add) const {
            return complex(this->real_part + add.real_part, this->imaginary_part + add.imaginary_part);
        }
        complex operator - (complex add) const {
            return complex(this->real_part - add.real_part, this->imaginary_part - add.imaginary_part);
        }
        complex operator * (complex add) const {
            return complex(this->real_part * add.real_part - this->imaginary_part * add.imaginary_part,
                              this->imaginary_part * add.real_part + this->real_part * add.imaginary_part);
        }
        complex operator / (complex add) const {
            return complex((this->real_part * add.real_part + this->imaginary_part * add.imaginary_part) / (add.real_part * add.real_part + add.imaginary_part * add.imaginary_part),
                              (this->imaginary_part * add.real_part - this->real_part * add.imaginary_part) / (add.real_part * add.real_part + add.imaginary_part * add.imaginary_part));
        }

        double radius() {
            return sqrt(this->real_part * this->real_part + this->imaginary_part * this->imaginary_part);
        }
};

#endif // MANDELBROT_H
