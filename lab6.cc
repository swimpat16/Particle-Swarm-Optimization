#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

void calculateStatistics(int);

class particle
{
    public:
        void init();
        
        double x_vel;
        double y_vel;
        double x_pos;
        double y_pos;
        double fitness;
        double best_fitness;
        double x_best;
        double y_best;
};

// Globals
int NUM_EPOCHS;
int NUM_PARTICLES;
double INERTIA;
double COGNITION;
double SOCIAL;
int WORLD_HEIGHT = 100;
int WORLD_WIDTH = 100;
double GLOBAL_MAX_X;
double GLOBAL_MAX_Y;
double ERROR_THRESHOLD = 0.001;
vector<particle> swarm;
particle global_best;
double MAX_VELOCITY;
int PROB_NUM;
vector< vector<double> > statistics;
string OUTFILE = "";
bool DEBUG = false;
bool IMG = false;

double calculatePError(particle p)
{
    return sqrt(pow(fabs(p.x_pos - GLOBAL_MAX_X), 2.0) + pow(fabs(p.y_pos - GLOBAL_MAX_Y), 2.0));
}

void initSwarm()
{
    global_best.fitness = 0.0;
    particle p;
    
    // Initialize GLOBAL MAX 
    if(PROB_NUM == 1)
    {
        GLOBAL_MAX_X = 20.0;
        GLOBAL_MAX_Y = 7.0;
    }
    else
    {
        GLOBAL_MAX_X = -20.0;
        GLOBAL_MAX_Y = -7.0;
    }
    
    // Initialize all particles in the swarm
    swarm.resize(NUM_PARTICLES);
    for(int i = 0; i < NUM_PARTICLES; i++)
    {
        swarm[i].init();
        
        // Keep track of fitness of most fit particle
        if (swarm[i].best_fitness > global_best.fitness)
        {
            global_best.x_pos = swarm[i].x_best;
            global_best.y_pos = swarm[i].y_best;
        }
    }
    
    statistics.resize(NUM_EPOCHS);
    for(int i = 0; i < NUM_EPOCHS; i++)
    {

        statistics[i].resize(8);
        for (int j = 0; j < statistics[i].size(); j++)
        {
            statistics[i][j] = 0.0;
        }
    }
}

double max(double a, double b)
{
    if (a > b)
        return a;
    return b;
}

// Max Distance
double mdist()
{
    return sqrt((pow((WORLD_WIDTH / 2.0), 2.0) + pow((WORLD_HEIGHT / 2.0), 2.0)) / 2.0);
}


double pdist(particle p)
{
    return sqrt(pow(fabs(p.x_pos - 20.0), 2.0) + pow(fabs(p.y_pos - 7.0), 2.0));
}


double ndist(particle p)
{
    return sqrt(pow(fabs(p.x_pos + 20.0), 2.0) + pow(fabs(p.y_pos + 7.0), 2.0));
}

// Calculate fitness for problem 1
double calc_fitness_1(particle p)
{
    return 100.0 * (1.0 - (pdist(p) / mdist()));
}

// Calculate fitness for problem 2
double calc_fitness_2(particle p)
{
    return (9.0 * max(0.0, 10.0 - pow(pdist(p), 2.0))) + (10.0 * (1.0 - (pdist(p) / mdist()))) + (70.0 * (1.0 - (ndist(p) / mdist())));
}

// Initialize particle
void particle::init()
{
    x_vel = 0.0;
    y_vel = 0.0;
    x_pos = 0.0;
    y_pos = 0.0;
    fitness = 0.0;
    best_fitness = 0.0;
    x_best = 0.0;
    y_best = 0.0;
    
    x_pos = (rand() % WORLD_WIDTH) - (WORLD_WIDTH / 2);
    y_pos = (rand() % WORLD_HEIGHT) - (WORLD_HEIGHT / 2);
    
    if(PROB_NUM == 1)
        fitness = calc_fitness_1(*this);
    else
        fitness = calc_fitness_2(*this);
        
    best_fitness = fitness;
    x_best = x_pos;
    y_best = y_pos;
    
    // Keep Global Fitness up to date
    if(fitness > global_best.fitness)
    {
        global_best.fitness = fitness;
        global_best.x_pos = x_pos;
        global_best.y_pos = y_pos;
    }
    
}


// Load constants from stdin or config file
void loadConstants(int argc, char ** argv)
{
    if (argc >= 8)
    {
        NUM_EPOCHS = atoi(argv[1]);
        NUM_PARTICLES = atoi(argv[2]);
        INERTIA = atof(argv[3]);
        COGNITION = atof(argv[4]);
        SOCIAL = atof(argv[5]);
        PROB_NUM = atoi(argv[6]);
        MAX_VELOCITY = atof(argv[7]);
        
        if (argc == 9)
            IMG = true;
    }
    else
    {
        if (argc <= 1)
        {
            cerr << "Usage: ./lab6 NUM_EPOCHS NUM_PARTICLES INERTIA COGNITION SOCIAL PROB_NUM MAX_VELOCITY [IMG]\n";
            cerr << "\tOR\n";
            cerr << "       ./lab6 config_filename [IMG]" << endl;
            exit(1);
        }
        
        ifstream infile(argv[1]);
        
        if (!infile.is_open())
        {
            cerr << "Invalid config file given" << endl;
            exit(1);
        }
        
        infile >> NUM_EPOCHS >> NUM_PARTICLES >> INERTIA >> COGNITION >> SOCIAL >> PROB_NUM >> MAX_VELOCITY;
        
        OUTFILE = argv[2];
        
        if (argc == 4)
        {
            IMG = true;
            cout << "Read image flag\n";
        }
    }
    
    if(PROB_NUM != 1 && PROB_NUM != 2)
    {
        cerr << "Problem Number must be either 1 or 2" << endl;
        exit(1);
    }
    
}



// Iterates each particle in the swarm once
void updateSwarm(int iterations)
{
    double r1, r2, r3, r4;
    
    for (int i = 0; i < swarm.size(); i++)
    {
        r1 = (rand() * 1.0) / (RAND_MAX * 1.0);
        r2 = (rand() * 1.0) / (RAND_MAX * 1.0);
        
        // Indecisiveness
        if (true)
        {
            r3 = (rand() * 1.0) / (RAND_MAX * 1.0);
            r4 = (rand() * 1.0) / (RAND_MAX * 1.0);
        }
        else
        {
            r3 = r1;
            r4 = r2;
        }
        
        // Calculate new velocity
        swarm[i].x_vel = INERTIA * swarm[i].x_vel + COGNITION * r1 * (swarm[i].x_best - swarm[i].x_pos) + SOCIAL * r2 * (global_best.x_pos - swarm[i].x_pos);
        swarm[i].y_vel = INERTIA * swarm[i].y_vel + COGNITION * r3 * (swarm[i].y_best - swarm[i].y_pos) + SOCIAL * r4 * (global_best.y_pos - swarm[i].y_pos);
        
        // Scale new velocity
        if (fabs(sqrt(pow(swarm[i].x_vel, 2.0) + pow(swarm[i].y_vel, 2.0))) > pow(MAX_VELOCITY, 2.0))
        {
            swarm[i].x_vel = (MAX_VELOCITY / sqrt(pow(swarm[i].x_vel, 2.0) + pow(swarm[i].y_vel, 2.0))) * swarm[i].x_vel;
            swarm[i].y_vel = (MAX_VELOCITY / sqrt(pow(swarm[i].x_vel, 2.0) + pow(swarm[i].y_vel, 2.0))) * swarm[i].y_vel;
        }
        
        // Calculate new position
        swarm[i].x_pos += swarm[i].x_vel;
        swarm[i].y_pos += swarm[i].y_vel;
        
        // Particles that move off the edge reappear on the other side
        if (swarm[i].x_pos < 0.0 - (WORLD_WIDTH / 2.0))
            swarm[i].x_pos += WORLD_WIDTH;
        if (swarm[i].x_pos > (WORLD_WIDTH / 2.0))
            swarm[i].x_pos -= WORLD_WIDTH;
        if (swarm[i].y_pos < 0.0 - (WORLD_HEIGHT / 2.0))
            swarm[i].y_pos += WORLD_HEIGHT;
        if (swarm[i].y_pos > (WORLD_HEIGHT / 2.0))
            swarm[i].y_pos -= WORLD_HEIGHT;
        
        // Calculate Fitness for Each Particle
        if (PROB_NUM == 1)
        {
            swarm[i].fitness = calc_fitness_1(swarm[i]);
        }
        else
        {
            swarm[i].fitness = calc_fitness_2(swarm[i]);
        }
        
        // Calculate Best Fitness for Each Particle
        if (swarm[i].fitness > swarm[i].best_fitness)
        {
            swarm[i].best_fitness = swarm[i].fitness;
            swarm[i].x_best = swarm[i].x_pos;
            swarm[i].y_best = swarm[i].y_pos;
        }
        
        // Calculate Globabl Best
        if (swarm[i].fitness > global_best.fitness)
        {
            global_best.fitness = swarm[i].fitness;
            global_best.x_pos = swarm[i].x_pos;
            global_best.y_pos = swarm[i].y_pos;
        }
    }
    
    calculateStatistics(iterations);
}

// Calculate average error of X coordinates
double calculateXError()
{
    double x_error = 0.0;
    
    for (int i = 0; i < swarm.size(); i++)
    {
        x_error += pow(fabs(swarm[i].x_pos - global_best.x_pos), 2.0);
    }
    
    x_error = sqrt(x_error / (2.0 * swarm.size()));
    return x_error;
}

// Calculate average error of Y coordinates
double calculateYError()
{
    double y_error = 0.0;
    
    for (int i = 0; i < swarm.size(); i++)
    {
        y_error += pow(fabs(swarm[i].y_pos - global_best.y_pos), 2.0);
    }
    
    y_error = sqrt(y_error / (2.0 * swarm.size()));
    return y_error;
}

// Calculate the error
double calculateError()
{
    return sqrt(pow(calculateXError(), 2.0) + pow(calculateYError(), 2.0));
}

// Print all data from run to csv
void printToCSV()
{
    std::ofstream realOutFile;

    if(OUTFILE != "")
        realOutFile.open(OUTFILE.c_str(), std::ios::out);

    std::ostream & outFile = (OUTFILE != "" ? realOutFile : std::cout);
    
    // Print out config variables
    outFile << "Number of Epochs, " << NUM_EPOCHS << "\n";
    outFile << "Number of Particles, " << NUM_PARTICLES << "\n";
    outFile << "Inertia, " << INERTIA << "\n";
    outFile << "Cognition, " << COGNITION << "\n";
    outFile << "Social, " << SOCIAL << "\n";
    outFile << "Problem Number, " << PROB_NUM << "\n";
    outFile << "Maximum Velocity, " << MAX_VELOCITY << "\n\n";
    
    // Print out statistics for each run
    outFile << "Epoch Number, Avg X Pos, Avg Y Pos, Percentage Within Error, Avg Fitness, Global Best Fitness, Global Best X Pos, Global Best Y Pos\n";
    for (int i = 0; i < statistics.size(); i++)
    {
        for (int j = 0; j < statistics[i].size() - 1; j++)
        {
            outFile << left << setw(12) << statistics[i][j] << ", ";
        }
        outFile << left << setw(12) << statistics[i][statistics[i].size() - 1] << "\n";
    }
    outFile << "\n";
    
    // Print out particle values at end of simulation
    outFile << "Particle Number, X Pos, Y Pos, Fitness\n";
    for (int i = 0; i < swarm.size(); i++)
    {
        outFile << right << setw(3) << i + 1 << ", ";
        outFile << left << setw(12) << swarm[i].x_pos << ", ";
        outFile << left << setw(12) << swarm[i].y_pos << ", ";
        outFile << left << setw(12) << swarm[i].fitness << "\n";
    }
}

void printEpochToDebug(int n, double error)
{
    ofstream outFile("debug.csv");
    
    // Print out particle values at end of each epoch
    outFile << "Epoch: " << n << "\n\n";
    outFile << "Particle Number, X Pos,        Y Pos,        Fitness,      Best Fitness,   X Vel,        Y Vel,     Perror\n";
    for (int i = 0; i < swarm.size(); i++)
    {
        outFile << left << setw(16) << i + 1 << ", ";
        outFile << left << setw(12) << swarm[i].x_pos << ", ";
        outFile << left << setw(12) << swarm[i].y_pos << ", ";
        outFile << left << setw(12) << swarm[i].fitness << ", ";
        outFile << left << setw(12) << swarm[i].best_fitness << ", ";
        outFile << left << setw(12) << swarm[i].x_vel << ", ";
        outFile << left << setw(12) << swarm[i].y_vel << ",";
        outFile << left << setw(12) << calculatePError(swarm[i]) << "\n";
    }
    outFile << "\n";
    
    //Print out Global Best
    outFile << "X Global, Y Global, Global Fitness, Global Perror, Global Error\n";
    outFile << left << setw(9) << global_best.x_pos << ", ";
    outFile << left << setw(9) << global_best.y_pos << ", ";
    outFile << left << setw(9) << global_best.fitness << ", ";
    outFile << left << setw(9) << calculatePError(global_best) << ", ";
    outFile << left << setw(9) << error;
    outFile << "\n\n\n";
    
}

// Prints each update to a ppm image file
void printToPPM(int n)
{
    // Create filename
    stringstream ss;
    ss << "img/frame";
    ss << setw(5) << setfill('0') << n;
    ss << ".pbm";
    string filename = ss.str();
    
    
    // Make vector to represent particles
    vector< vector<int> > field;
    field.resize(WORLD_HEIGHT * 10);
    for(int i = 0; i < WORLD_HEIGHT * 10; i++)
    {
        field[i].resize(WORLD_WIDTH * 10);
        for(int j = 0; j < WORLD_WIDTH * 10; j++)
        {
            field[i][j] = 0;
        }
    }
    
    // Loop through particles
    for(int i = 0; i < swarm.size(); i++)
    {
        int x_index = ((int) ((swarm[i].x_pos + 50.0) * 10.0)) - 2;
        int y_index = ((int) ((swarm[i].y_pos + 50.0) * 10.0)) - 2;
        for(int j = 0; j < 5; j++)
        {
            for(int k = 0; k < 5; k++)
            {
                if (y_index + j >= 0 && y_index + j < WORLD_HEIGHT * 10 && x_index + k >= 0 && x_index + k < WORLD_WIDTH * 10)
                    field[y_index + j][x_index + k] = 1;
            }
        }
    }
    
    
    // Print vector to ppm image
    
    std::ofstream outFile;
    outFile.open(filename.c_str(), std::ios::out);    

    // Print Header
    outFile << "P1" << endl;
    outFile << (WORLD_WIDTH * 10) << " " << (WORLD_HEIGHT * 10) << endl;
    
    // Print Images
    // Rows
    for(int i = 0; i < field.size(); i++)
    {
        // Cols
        for(int j = 0; j < field[i].size(); j++)
        {
            // Pixel
            outFile << field[i][j] << " ";
        }
        outFile << "\n";
    }

    // Close stream
    outFile.close();
}

// Main loop, runs simulation
void runSimulation()
{
    int iterations = 0;
    double error = 1.0;
    
    if(DEBUG) printEpochToDebug(iterations, error);
    if(IMG) printToPPM(iterations);
    
    while(error > ERROR_THRESHOLD && iterations < NUM_EPOCHS)
    {
        updateSwarm(iterations);
        
        iterations++;
        error = calculateError();
        if(DEBUG) printEpochToDebug(iterations, error);
        if(IMG) printToPPM(iterations);
    }
    
    // Avoid Seg Faults
    if (iterations < NUM_EPOCHS)
        statistics.resize(iterations);
    
    printToCSV();
}

void calculateStatistics(int n)
{
    
    // Epoch Number
    statistics[n][0] = ((double) n);
    
    // Average X
    // Average Y
    // Percentage of Particles Within 0.001
    // Average Fitness
    double x_average = 0.0;
    double y_average = 0.0;
    int count = 0;
    double fitness_average = 0.0;
    
    for(int i = 0; i < swarm.size(); i++)
    {
        x_average += swarm[i].x_pos;
        y_average += swarm[i].y_pos;
        if (calculatePError(swarm[i]) < ERROR_THRESHOLD)
            count++;
        fitness_average += swarm[i].fitness;
    }
    statistics[n][1] = x_average / (double)swarm.size();
    statistics[n][2] = y_average / (double)swarm.size();
    statistics[n][3] = ((double) count) / ((double) swarm.size());
    statistics[n][4] = fitness_average / (double)swarm.size();
    
    // Global Best Fitness
    statistics[n][5] = global_best.fitness;
    
    // Global Best X
    statistics[n][6] = global_best.x_pos;
    
    // Global Best Y
    statistics[n][7] = global_best.y_pos;
}

void resetDebug()
{
    ofstream outfile("debug.csv", ios::out);
    outfile.close();
}

int main(int argc, char ** argv)
{
    srand(time(NULL));
    
    if (DEBUG)
        resetDebug();
    
    loadConstants(argc, argv);
    initSwarm();
    
    runSimulation();
    
    if (IMG)
    {
        cout << "IMG true\n";
    }
    else
    {
        cout << "IMG false\n";
    }
}