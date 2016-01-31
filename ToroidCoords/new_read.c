#include <stdio.h>
#include <math.h>

int main()
{
  FILE   *fp,*fp1,*fp2;

  int    i,iprobe,icount=0,ifile,n,j,stop,jmax;
  int    i1,i2,i3,i4,i5,i6,i7,i8,i9,i10;
  int    i11,i12,i13,i14,i15,i16,i17,i18,i19,i20,i21,i22;
  float  f1,f,z,dt,time,z1,z2,ang1,ang2,freq,freqp,ang2p;
  float  probedat[50000][8];

  double a[501],b[500];              // field fourier coefficients
  double PI;
  double theta,dtheta,theta_tot,sum;
  double frecon[20000];             //  reconstructed field

  PI = acos(-1.0);
  jmax = 10000;                     // number of reconstructed field points 

  ifile = 1;

  if (ifile==1) fp = fopen("trolley_9_4_56_28_4_101_enc_asc.dat","r");   // file 1
  if (ifile==2) fp = fopen("trolley_8_40_41_12_3_101_enc_asc.dat","r");  // file 2

  while (fscanf(fp,"%d %d %f %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
                &i1,&i2,&f1,&i3,&i4,&i5,&i6,&i7,&i8,&i9,&i10,&i11,&i12,&i13,&i14,
  		&i15,&i16,&i7,&i18,&i19,&i20,&i21,&i22)!=EOF){

    time   = f1;
    iprobe = i13;
    dt     = (float)i15;           // time in units of 61.74 MHz 
    z      = (float)i16;           // number of zero crossings
    z1     = i21;
    z2     = i22;

    if ((z>0) && (dt>0)){
      f = (61.74E6*z)/dt;          // mixed-down frequency of FID
    } else {
      f = -1;
    }

    if (iprobe == 1){  
      ang1 = -999;  ang2 = -999;
      if (ifile==1){
        if ((z1<150000) && (z1>0)) ang1=z1*360.0/107568.0;  // 
        if ((z2<150000) && (z2>0)) ang2=z2*360.0/108140.0;  // 
      } else if (ifile==2){
        if ((z1<150000) && (z1>0)) ang1=z1*360.0/108081.0;  // 
        if ((z2<150000) && (z2>0)) ang2=z2*360.0/108312.0;  // 
      }

      // study of files 1 and 2 suggest z2 is more accurate - use ang2

      probedat[icount][0]=z;
      probedat[icount][1]=dt;
      probedat[icount][2]=f;
      probedat[icount][3]=time;
    
      probedat[icount][4]=z1;  probedat[icount][5]=z2;
      probedat[icount][6]=ang1;
      probedat[icount][7]=ang2;
      icount = icount + 1;
    }
  }
  
  fclose(fp);

  if (ifile==1) fp = fopen("trolley1.out","w");
  if (ifile==2) fp = fopen("trolley2.out","w");
  fp1 = fopen("trolley3.out","w");

  for (i=0; i<icount; i++){
    fprintf(fp,"%f %f %f %f %f %f %f %f\n",probedat[i][3],probedat[i][2],probedat[i][0],probedat[i][1],
	    probedat[i][4],probedat[i][5],probedat[i][6],probedat[i][7]);
    if ((probedat[i][2]>0)&&(probedat[i][7]>-100)){
       fprintf(fp1,"%f %f\n",probedat[i][7],probedat[i][2]);
    }
  }
  fclose(fp);
  fclose(fp1);

  // do analysis : 
  // first, assume correct for shift in B during trolley run, assuming linear freq(t=0,ang=0)=freq(t=T,ang=0)
  // next, extract fourier coefficients

  for (n=0; n<=6*36; n++){
    a[n] = 0.0;  b[n] = 0.0; theta_tot = 0.0;
    for (i=0; i<icount-1; i++){
      ang2 = probedat[i][7];
      freq = probedat[i][2];
      if ((ang2>=0.0)&&(ang2<=360.0)&&(freq>0.0)){     // good measurement

        stop = 0;
        j = i+1;
        while (stop==0){                               // find next good measurement
          ang2p = probedat[j][7];
          freqp = probedat[j][2];
          if ((ang2p>=0.0)&&(ang2p<=360.0)&&(freqp>0.0)){        // good measurement
	    dtheta = (ang2p-ang2)*2.0*PI/360.0;
            stop = 1;
          } else {
            j = j+1;
          }
          if (j>i+11){
            printf("Good measurements too far apart %d %f %f\n",j-i,ang2,theta_tot);
            stop = 1;
          }
        }

        theta = ang2*2.0*PI/360.0;
        a[n]=a[n]+freq*cos(n*theta)*dtheta;
        b[n]=b[n]+freq*sin(n*theta)*dtheta;
        theta_tot = theta_tot + dtheta;
      }
      if (fabs(theta_tot) >= 2.0*PI*(1.0-5.0e-5)) i=icount;
    }
    if (dtheta<0.0){
     a[n] = -a[n]/PI;
     b[n] = -b[n]/PI;
    } else {
     a[n] =  a[n]/PI;
     b[n] =  b[n]/PI;
    }
    printf("%d %lf\n",n,sqrt(a[n]*a[n]+b[n]*b[n]));
  }

    // reconstruct field

  if (ifile==1) fp2 = fopen("recon1.out","w");
  if (ifile==2) fp2 = fopen("recon2.out","w");
  for (j=0; j<jmax; j++){
    frecon[j] = a[0]/2.0;
    ang2 = (j*2.0*PI)/jmax;
    for (n=1; n<=6*36; n=n+1){
      frecon[j]=frecon[j]+a[n]*cos(n*ang2)+b[n]*sin(n*ang2);
    }
    fprintf(fp2,"%d %lf %lf\n",j,ang2*360.0/(2.0*PI),frecon[j]);
  }
  fclose(fp2);

  // predicted field average 

  theta_tot = 0.0;
  jmax = 6000;
  for (j=0; j<jmax; j++){
    sum = a[0]/2.0;
    ang2 = ((j+0.5)*2.0*PI)/jmax;
    //    for (n=1; n<=6*36; n=n+1){
    for (n=1; n<=6*36; n=n+1){
      sum=sum+(a[n]*cos(n*ang2)+b[n]*sin(n*ang2))*(2.0*PI/jmax);
    }
    theta_tot = theta_tot+(2.0*PI/jmax);
  }
  printf("Field average = %lf versus a[0]/2=%lf theta_tot=%lf\n",sum/61.74,a[0]/2.0/61.74,theta_tot-2.0*PI);

  return;
}
