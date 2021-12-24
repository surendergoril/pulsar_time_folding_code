using namespace std;
#include<iostream>
#include<string>
#include<math.h>
#include<fstream>
#include<iomanip>

ofstream fout("listsSNR.txt");
int main()
{
double arr1[1000],arr2[1000],arr3[1000],arr4[1000],y,b,k,a;
int x;
cout<<"\nMean_off_pulse\t"<<"Off_pulse_RMS\t"<<"Mean_by_RMS\t"<<"SNR_before_normalization\t"<<"RMS_noise_value_AN"<<"\t"<<"SNR_value_AN"<<"\t"<<"file name with parameters"<<"\n\n\n";
fout<<"\nMean\t"<<"RMS noise value"<<"\t\t"<<"SNR value"<<"\t"<<"file name with parameters"<<"\n\n\n";
ifstream fin1;
ifstream fin;
fin.open("fnames.txt");
string line;
ofstream fout1;
int i,d;
i=0;
while(!fin.eof())
{
	getline(fin,line);          
	fin>>line;
	//cout<<line<<"\n";
	fin1.open(line.c_str());
	{
		string row;
		d=0;
		while (true)
		{
			getline (fin1,row);                                       //checking for #
			if(row[0]!='#')
			{
				fin1>>x>>y;
				arr1[d]=x;arr2[d]=y;
				//cout<<arr1[d]<<"\t"<<arr2[d]<<"\n";
				d=d++;
				k=d;
			}

			if(fin1.eof())
			{
			cout<<"END OF FILE\n";
			break;
			}
		}
	}
//cout<<k<<"\n";
fin1.close();
b=arr2[0];
//cout<<b<<"\n";
for(int j=0;j<(k-1);j++)                       // searching for maxima
{
	if(b<=arr2[j+1])
	{
		b=arr2[j+1];
		a=arr1[j+1];
	}
}
//cout<<b<<"\n"<<a<<"\n";
double mean;
mean=0;
double max,min;

max=ceil(a+5*k/100);                      //finding the 5% range above the peak
min=floor(a-5*k/100);                     //finding the 5% range below the peak
/*
cout<<"The max is "<<max<<"\n";
cout<<"The min is "<<min<<"\n";
*/
for(int p=0;p<k;p++)                           // creating a mask
{
	if(p<min || p>max)
	{
		arr4[p]=1;
	}
	else
	{
		arr4[p]=0;
	}
	// cout<<arr4[p]<<"\n";
}
for(int p=0;p<k;p++)                           // calculating noise mean
{
	mean+=arr4[p]*arr2[p]/(k-(max-min+1));          // excluding the 5% of the bins around the peak to calculate mean by using mask
}
//cout<<"Mean is "<<mean<<"\n";

double rms,prms;                                    //calculation of the rms noise
rms=0;prms=0;
line.replace (41,0,"profile_");
fout1.open(line.c_str());
for(int p=0;p<k;p++)
{
	arr3[p]=(arr2[p]-mean)/(b-mean);              //normalizing the profile
	rms+=arr4[p]*arr3[p]*arr3[p]/(k-(max-min+1));        //excluding the bins around the peak for mean square noise
	prms+=arr4[p]*arr2[p]*arr2[p]/(k-(max-min+1));        // RMS for the raw data in arbitrary units
	//cout<<line<<"\n";
	fout1<<arr1[p]<<"\t"<<arr3[p]<<"\n";               // creating the file for normalized profile
	//cout<<arr3[p]<<"\n";
}
fout1.close();
cout<<setprecision(10)<<mean<<"\t"<<sqrt(prms-mean*mean)<<'\t'<<mean/sqrt(prms-mean*mean)<<'\t'<<(b-mean)/sqrt(prms-mean*mean)<<"\t"<<sqrt(rms)<<"\t"<<1/sqrt(rms)<<"\t"<<line<<"\n";
fout<<setprecision(10)<<mean<<"\t\t"<<sqrt(rms)<<"\t\t"<<1/sqrt(rms)<<"\t"<<line<<"\n";




/*if(fin.eof())
{
cout<<"END OF FNAMES\n";
break;
}
*/
i=i++;
}
fin.close();
return 0;
}


