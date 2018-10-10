#include <iostream>
#include <vector>
#include <climits>
#include <random>
#include <cmath>
#include <ctime>
#include <fstream>

using namespace std;



double AvgEnergy(double T,int nsites,int nsteps,int u_spin)
{
default_random_engine generator1;
default_random_engine generator2;
uniform_int_distribution<int> rand_nsite(0,nsites-1);
uniform_real_distribution<double> rand_fraction(0.0,1.0);
uniform_real_distribution<double> rand_fraction1(0.0,1.0);
short int spin[nsites];
double dE,AE=0;
int rand_site,q=0;
double E=1000-2*u_spin;

for(int i=0;i<u_spin;i++)
{spin[i]= 1;}
for(int i=u_spin;i<nsites;i++)
{spin[i]=-1;}
for(int i=1;i<=nsteps;i++)
{
	dE=0;
	rand_site=rand_nsite(generator1);
	
	if(spin[rand_site]==-1)
	{
		dE=-2;
		E=E+dE;
		spin[rand_site]=1;
	}
	
	else
	{
		dE=2;
		if(exp(-dE/T)>rand_fraction(generator2))
		{	
			E=E+dE;
			spin[rand_site]=-1;
		}
		
	}
	
	
	if(i>0.9*nsteps)
	{
	AE=(q*AE+E)/(q+1);
	q++;
	}	
}
return AE;	

}




int main()
{
cout<<"Hello Monte Carlo"<<endl;

fstream fs;
fs.open ("carlo.dat", fstream::in | fstream::out | fstream::app);
fs << " more lorem ipsum";



int nsteps=1000000;
int nsites=1000;
double Ti=0;
double dT=0.1;
double Tf=10;
int nT=100;
double E[nT];
clock_t b=clock();
double T=Ti;
int u_spin=1000;
double error=0;
for(int i=0;i<100;i++)	
{
	
	E[i]=AvgEnergy(T,nsites,nsteps,u_spin);
	fs<<T<<','<<-nsites*tanh(1/T)<<','<<E[i]<<'\n';
	cout<<T<<','<<-nsites*tanh(1/T)<<','<<E[i]<<'\n';
	T=T+dT;
	u_spin=(1000+E[i])/2;
	
}

clock_t e=clock();
cout<<double(e-b)/CLOCKS_PER_SEC<<endl;
fs.close();
return 0;
}



