#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

int main()
{
    int count = 0;
    
    int num_epochs = 1000;
    string num_particles[] = {"10", "20", "30", "40", "50", "100"};
    string inertia[] = {"0.1", "0.2", "0.35", "0.5", "0.70", "0.85", "0.95"};
    string cog[] = {"0.5", "1.25", "1.75", "2", "2.25", "2.75", "3.5", "4.5"};
    string soc[] = {"0.5", "1.25", "1.75", "2", "2.25", "2.75", "3.5", "4.5"};
    string problem[] = {"1", "2"};
    string max_vel[] = {"0.5", "1.5", "2.5", "3.5", "5", "7", "10"};
    
    
    
    // Number of Particles
    for(int i = 0; i < 6; i++)
    {
        // Inertia
        for(int j = 0; j < 7; j++)
        {
            // Cognition
            for(int k = 0; k < 8; k++)
            {
                // Social
                for(int l = 0; l < 8; l++)
                {
                    // Problem Number
                    for(int m = 0; m < 2; m++)
                    {
                        // Max Velocity
                        for(int n = 0; n < 7; n++)
                        {
                         
                            //Open and name config file
                            char filename[100];
                            sprintf(filename, "config/config%d.cf", count++);
                            ofstream outfile(filename, ofstream::out);
                        
                            //Make sure config file opened
                            if (!outfile.is_open())
                            {
                                cerr << "Cannot open " << filename << endl;
                                exit(1);
                            }
                            
                            // Print Number of Epochs
                            outfile << num_epochs << "\n";
                            
                            // Print Number of Particles
                            outfile << num_particles[i] << "\n";
                            
                            // Print Inertia
                            outfile << inertia[j] << "\n";
                            
                            // Print Cognition Parameter
                            outfile << cog[k] << "\n";
                            
                            // Print Social Parameter
                            outfile << soc[l] << "\n";
                            
                            // Print Problem Number
                            outfile << problem[m] << "\n";
                            
                            // Pring Max Velocity
                            outfile << max_vel[n];
                            
                            // Close outfile
                            outfile.close();
                            
                        }
                        
                    }
                    
                }
                
            }
            
        }
        
    }
    
   
    return 0;
}