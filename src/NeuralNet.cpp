#include "NeuralNet.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <chrono>

double(*functions[])(double inputs) = {
	[](double input) {//sigmoid
		return 1.0 / (1.0 + (double)std::exp(-1.0 * input));
	},
	[](double input) {//hyperbolic tangent
		return (double)std::tanh(input);
	},
	[](double input) {//relu
		return input >= 0.0 ? input : 0.0;
	},
    [](double input) {//leaky relu
        return input > 0.0 ? input : 0.01 * input;//0.01 needs to be a variable that can be changed externally...
    },
	[](double input) {//sigmoid derivative
		return input * (1.0 - input);
	},
	[](double input) {//hyperbolic tangent derivative
		return 1.0 - (double)std::pow(input, 2);
	},
	[](double input) {//relu derivative
		return input >= 0.0 ? 1.0 : 0.0;
	},
    [](double input) {//leaky relu derivative
        return input > 0.0 ? 1.0 : 0.01;
    }
};

//need to work on these randomization functions
double randomSig(int numInOut) {//i'll figure out how to determine better starting weights eventually
	double num = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
	return num * 2 - 1;
}

double randomTanh(int numInOut) {
	double num = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX) ;
	return num * 2 - 1;
}

double randomReLU(int numLayers) {
	double num = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
	//double num = std::sqrt(2.0f / (float) (std::rand() % numLayers ));
	return num * 2 - 1;
}

double(*randomize[])(int numInOut) = {
	randomSig,
	randomTanh,
	randomReLU,
    randomReLU
};

NeuralNet::NeuralNet(int numInputs, int numNodesPerLayer, int numHiddenLayers, int numOutputs, double step, int function) {
	if (function < 0 || function >= numFunctions) {
		function = 0;
	}
	outputSize = numOutputs;
	hiddenSize = numNodesPerLayer;
	inputSize = numInputs;
	learningRate = step;
	numWeights = numHiddenLayers + 1;

	weights = new Matrix<>*[numHiddenLayers + 1];
	weights[0] = new Matrix<>(numNodesPerLayer, numInputs);
	for (int i = 0; i < weights[0]->getHeight(); i++) {
		for (int j = 0; j < weights[0]->getWidth(); j++) {
			weights[0]->set(i, j, randomize[function](numHiddenLayers + 2));
		}
	}
	weights[numHiddenLayers] = new Matrix<>(numOutputs, numNodesPerLayer);
	for (int i = 0; i < weights[numHiddenLayers]->getHeight(); i++) {
		for (int j = 0; j < weights[numHiddenLayers]->getWidth(); j++) {
			weights[numHiddenLayers]->set(i, j, randomize[function](numHiddenLayers + 2));
		}
	}
	for (int i = 1; i < numHiddenLayers; i++) {
		weights[i] = new Matrix<>(numNodesPerLayer, numNodesPerLayer);
		for (int j = 0; j < weights[i]->getHeight(); j++) {
			for (int k = 0; k < weights[i]->getWidth(); k++) {
				weights[i]->set(j, k, randomize[function](numHiddenLayers + 2));
			}
		}
	}

	biases = new Matrix<>*[numHiddenLayers + 1];
	for (int i = 0; i < numHiddenLayers; i++) {
		biases[i] = new Matrix<>(numNodesPerLayer, 1);
		for (int j = 0; j < biases[i]->getHeight(); j++) {
			biases[i]->set(j, 0, randomize[function](numInputs + numOutputs));
		}
	}
	biases[numHiddenLayers] = new Matrix<>(numOutputs, 1);
	for (int i = 0; i < biases[numHiddenLayers]->getHeight(); i++) {
		biases[numHiddenLayers]->set(i, 0, randomize[function](numInputs + numOutputs));
	}

	activationFunction = functions[function];
	dActivationFunction = functions[function + (int)numFunctions];
}

NeuralNet::~NeuralNet() {
	for (int i = 0; i < numWeights; i++) {
		delete weights[i];
		delete biases[i];
	}
	delete[] weights;
	delete[] biases;
}


Matrix<double> NeuralNet::feedForward(Matrix<double> inputs) {
	Matrix<> outputs(weights[0]->multiply(inputs));
	outputs.add(*biases[0], true);
	outputs.map(activationFunction, true);
	for (int i = 1; i < numWeights; i++) {
		outputs.copy(weights[i]->multiply(outputs));
		outputs.add(*biases[i], true);
		outputs.map(activationFunction, true);
	}

	return outputs;
}

Matrix<double> NeuralNet::feedForward(double* inputs) {//overload converts input array to matrix
	Matrix<double> inputMatrix(inputSize, 1);
	for (int i = 0; i < inputSize; i++) {
		inputMatrix.set(i, 0, inputs[i]);
	}
	return feedForward(inputMatrix);
}

void NeuralNet::train(Matrix<double> inputs, Matrix<double> expected) {
	//double momentum = .0001;
	Matrix<double> **hidden = new Matrix<>*[numWeights];
	
	
	//calculate output, storing each layer along the way
	hidden[0] = new Matrix<>(weights[0]->multiply(inputs));
	hidden[0]->add(*biases[0], true);
	hidden[0]->map(activationFunction, true);
	for (int i = 1; i < numWeights; i++) {
		hidden[i] = new Matrix<>(weights[i]->multiply(*hidden[i - 1]));
		hidden[i]->add(*biases[i], true);
		hidden[i]->map(activationFunction, true);
	}
	
	//calculate error at each layer
	//calculate gradient at each layer
	//calculate deltas for each bias and weight
	//rinse and repeat
	Matrix<> error(expected.subtract(*hidden[numWeights-1]));
	Matrix<> gradient(hidden[numWeights-1]->map(dActivationFunction));
	gradient.multiply(error, true);
	gradient.multiply(learningRate, true);
	biases[numWeights - 1]->add(gradient, true);
	Matrix<> deltas(gradient.multiply(hidden[numWeights-2]->transpose()));
	weights[numWeights - 1]->add(deltas, true);
	for (int i = numWeights - 2; i > 0; i--) {
		error.add(weights[i]->transpose().multiply(error));
		gradient.copy(*hidden[i]);
		gradient.map(dActivationFunction);
		gradient.multiply(error, true);
		gradient.multiply(learningRate, true);
		biases[i]->add(gradient, true);
		Matrix temp(*hidden[i - 1]);
		deltas.copy(gradient.multiply(temp.transpose()));
		weights[i - 1]->add(deltas, true);
		//weights[i - 1]->add(weights[i]->multiply(momentum), true);
	}

	for (int i = 0; i < numWeights; i++) {
		delete hidden[i];
	}
	delete[] hidden;
}
