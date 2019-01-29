#pragma once
#include "Matrix.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <chrono>

extern double(*functions[])(double);
extern double(*randomize[])(int);

class NeuralNet {
public:
	NeuralNet(int numInputs, int numNodesPerLayer, int numHiddenLayers, int numOutputs, double step = 0.01, int function = tanh); //function defaults to sigmoid
	~NeuralNet();

	Matrix<double> feedForward(Matrix<double> inputs);
	Matrix<double> feedForward(double* inputs);

	void train(Matrix<double> inputs, Matrix<double> expected);
	void train(Matrix<double> inputs, double* expected);
	void train(double* inputs, Matrix<double> expected);
	void train(double* inputs, double* expected);

	enum functionType {
		sigmoid,
		tanh,
		relu,
                relu_leaky,
                //softmax,
		numFunctions
	};

private:
	Matrix<> **weights;
	Matrix<> **biases;

	double learningRate;
	double (*activationFunction)(double input);
	double (*dActivationFunction)(double input);
	
	int numWeights;
	int inputSize;
	int hiddenSize;
	int outputSize;
};
