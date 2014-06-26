#include <OpenANN/OpenANN>
#include <OpenANN/io/Logger.h>
#include <OpenANN/io/DirectStorageDataSet.h>
#include <OpenANN/util/Random.h>
#include <Eigen/Core>
#include <iostream>
#include "tinyxml2.h"

using namespace OpenANN;
using namespace tinyxml2;


const int num_rates = 8;
const int rates[] = {6, 9, 12, 18, 24, 36, 48, 54};
int categorize(double input)
{
	double min_dst = 99.; int last_rate = -1;
	for (int i = 0; i < num_rates; i++)
	{
		float dst = abs(((float)rates[i]) - input);
		if (dst <= min_dst) {min_dst = dst; last_rate = rates[i];}
	}
	return last_rate;
}



int main(int argc, const char* argv[])
{
	// Set constants
	const int D = 3; // number of inputs
	const int F = 1; // number of outputs
	
	if (argc < 2) {std::cout << "Usage: ann-train filename" << std::endl; exit(0);}



	// load dataset from XML
	XMLDocument doc;
	doc.LoadFile( argv[1] );
	doc.PrintError();

	XMLElement* datasetElement = doc.FirstChildElement("data_set");
	if (!datasetElement) {std::cout << "ERROR traversing XML: dataset child not found" << std::endl; exit(-1);}

	// count number of links -> size of training set data
	int N = 0;
	for (XMLElement* child = datasetElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
	{
		if (strcmp(child->Name(), "link") == 0) N += 1;
			else {std::cout << "XML wrong format ERROR: A child of data_set was not a link" << std::endl; exit(-1);}
	}

	// create matrices for data 
	Eigen::MatrixXd X(N, D); // inputs
	Eigen::MatrixXd T(N, F); // desired outputs (targets)

	// load data from XML DOM
	int i = 0;
	for (XMLElement* child = datasetElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
	{
		int num_neighbours, num_hidden_nodes, rssi, optimal_rate;

		XMLElement* elem_num_neighbours = child->FirstChildElement("number_of_neighbors");
		if (!elem_num_neighbours) {std::cout << "ERROR traversing XML: number_of_neighbors child not found" << std::endl; exit(-1);}
		if (elem_num_neighbours->QueryIntText(&num_neighbours) != XML_SUCCESS) {std::cout << "ERROR traversing XML: number_of_neighbors not an int value" << std::endl; exit(-1);}

		XMLElement* elem_num_hidden_nodes = child->FirstChildElement("number_of_hidden_nodes");
		if (!elem_num_hidden_nodes) {std::cout << "ERROR traversing XML: number_of_hidden_nodes child not found" << std::endl; exit(-1);}
		if (elem_num_hidden_nodes->QueryIntText(&num_hidden_nodes) != XML_SUCCESS) {std::cout << "ERROR traversing XML: number_of_hidden_nodes not an int value" << std::endl; exit(-1);}

		XMLElement* elem_rssi = child->FirstChildElement("rssi");
		if (!elem_rssi) {std::cout << "ERROR traversing XML: rssi child not found" << std::endl; exit(-1);}
		if (elem_rssi->QueryIntText(&rssi) != XML_SUCCESS) {std::cout << "ERROR traversing XML: rssi not an int value" << std::endl; exit(-1);}

		XMLElement* elem_optimal_rate = child->FirstChildElement("optimal_rate");
		if (!elem_optimal_rate) {std::cout << "ERROR traversing XML: optimal_rate child not found" << std::endl; exit(-1);}
		if (elem_optimal_rate->QueryIntText(&optimal_rate) != XML_SUCCESS) {std::cout << "ERROR traversing XML: optimal_rate not an int value" << std::endl; exit(-1);}

		//std::cout << "Dataset " << i <<": " << num_neighbours << ", " << num_hidden_nodes << ", " << rssi << ": " << optimal_rate << std::endl;
		
		X.row(i) << num_neighbours, num_hidden_nodes, rssi;
		T.row(i) << optimal_rate / 54.;
		
		i++;
	}

	// Store matrices into dataSet
	DirectStorageDataSet dataSet(&X, &T);






	// Make the result repeatable
	RandomNumberGenerator().seed(0);

	// Create network
	Net net;
	// Add an input layer with D inputs, 1 hidden layer with 2 nodes and an
	// output layer with F outputs. Use logistic activation function in hidden
	// layer and output layer.
	makeMLNN(net, LOGISTIC, LOGISTIC, D, F, 3, 4, 4, 2);
	// Add training set
	net.trainingSet(dataSet);

	// Set stopping conditions
	StoppingCriteria stop;
	stop.minimalValueDifferences = 1e-20;
	// Train network, i.e. minimize sum of squared errors (SSE) with
	// Levenberg-Marquardt optimization algorithm until the stopping criteria
	// are satisfied.
	train(net, "LMA", MSE, stop);
	
	std::cout << "Training finished." << std::endl;

	// Use network to predict labels of the training data
	for(int n = 0; n < N; n++)
	{
		Eigen::VectorXd y = net(dataSet.getInstance(n));
		std::cout << "Dataset " << n << ": \t" << X.row(n)[0] << ", " << X.row(n)[1] << ", " << X.row(n)[2] << "; \t\t " << 
		"opt rate: " << T.row(n)*54. << " \t ann rate: " << categorize(y[0]*54.) << "\t ann raw: " << y*54. <<std::endl;
	}
	
	// Try something else on the ann
	Eigen::VectorXd newData(D);
	newData << 6.0, 0.0, 14.0;
	
	Eigen::VectorXd y2 = net(newData);
	std::cout << "Different, unlearned data (RSSI 14):" << std::endl<< y2*54. << " \t\t  Kategorie: " << categorize(y2[0]*54.) << std::endl;
	
	
	// print out net
	//std::cout << "Net:" <<  std::endl;
	//net.save(std::cout); 

	return 0;
}
