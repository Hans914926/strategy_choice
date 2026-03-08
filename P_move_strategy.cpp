#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

main(){
	int n=5;
	int n2=20;
	int i,j,k;
	int b[n+5];
	int num,den;
	double q[2*n+1]={0};
	double P[2*n]={0};//原始機率 
	double P1[2*n][n2+1]={0};//會變的機率 
	double P1_true[2*n]={0};
	double P2[2*n]={0};//比較殘差用
	double random[n+1];
	double random_sum;
	int different=0;
	double d;
	double e=1;
	double P_M; 
	double L[n+1][n+1];
	double mess[n+1];
	double c[n+1]={0};
	double J[n+1][n+1];
	double sum;
	double v[n+1]={1.0};
	double Jv[n+1];
	double norm;
	double lambda=0;
	double lambda_old;
	double f=1.0;
	int rank;
	int rounds_max[n+1][n+1];//單場最大再來一場數 
	int rounds_best[n+1][n+1]={0};//最佳再來一場數 
	int rounds_again[n+1][n+1][2];//迴圈跑的再來一場數(0和rounds_max) 
	int max;//初始最大再來一場數 
	double E=0;
	double E_num=0;
	double E_den=0;
	double E_max[n+1]={0};
	int count=0;
	
	for(i=1;i<=n;i++){ 
		for(j=1;j<=n;j++){
			rounds_again[i][j][0]=0;
		}
	}
	
	for(i=1;i<=n;i++){//初始P 
		printf("等級%d出現機率(分子、分母)\n",i);
		scanf(" %d %d",&num,&den);
		P[i]=(double)num/den;
	}
	
	printf("最大再來一場場數?\n");//初始M 
	scanf("%d",&max);

	
	
	while(count<10){//策略變化數 
		e=1;
		for(i=1;i<=n;i++){//找L 
			for(j=1;j<=n;j++){
				if(rounds_best[i][j]==0){
					L[i][j]=1;
				}
				else{
					L[i][j]=1.0/(1.0+max);
				}
			}
		}
		
		for(i=1;i<=n;i++){//隨機P1 
			for(j=1;j<=n;j++){
				if(i==j){
					P1[i][j]=0.99;
				}
				else{
					P1[i][j]=0.0025;
				}
			}
		}
		for(i=1;i<=n;i++){
			P1[i][6]=0.2;
		}
		
		srand(time(NULL));
		for(k=n+2;k<=n2;k++){
			sum=0;
			for(i=1;i<=n;i++){
				random[i]=(double)rand()/RAND_MAX; 
				random_sum+=random[i];
			}
			for(i=1;i<=n;i++){
				P1[i][k]=random[i]/random_sum;
			}
		}	
			
		for(k=1;k<=n2;k++){
			e=1;
			
			while(e>0.00000000001){//迭代過程 
				for(i=1;i<=n;i++){
						P2[i]=P1[i][k];
				}
				
				
				for(i=1;i<=n;i++){//mess初始化 
					mess[i]=1;
					for(j=1;j<=n;j++){
						mess[i]+=P1[j][k]/L[i][j];
					}
				}
				
				P_M=0;
				for(i=1;i<=n;i++){
					P_M+=P[i]/mess[i];
				} 
				
				for(i=1;i<=n;i++){//算新的P 
						P1[i][k]=P[i]/(P_M*mess[i]);
				}
				e=0;
				for(i=1;i<=n;i++){
					e+=pow(P1[i][k]-P2[i],2.0);
				}
				e=sqrt(e);
			}
		}
		different=0;
		for(i=1;i<=n;i++){
			for(k=1;k<=n2;k++){
				for(j=1;j<=n2;j++){
					f=fabs(P1[i][k]-P1[i][j]);
					if(f>0.000000001){
						different=1;
					}
				}
			}
		}
		if(different==0){
			for(i=1;i<=n;i++){
				P1_true[i]=P1[i][6];
			}
		}
		
		
		for(k=1;k<=n;k++){//算雅可比矩陣用 
			c[k]=0; 
			for(i=1;i<=n;i++){
				c[k]+=-P[i]/(L[i][k]*mess[i]*mess[i]);
			}
		}
		
		for(i=1;i<=n;i++){
			for(j=1;j<=n;j++){
				J[i][j]=-P[i]*(c[j]*mess[i]+(1/L[i][j])*P_M)/(P_M*P_M*mess[i]*mess[i]);
			}
		}
		
		sum=0;
		for(i=1;i<=n;i++){
			for(j=1;j<=n;j++){
				sum+=fabs(J[i][j]);
			}
		}
		if(sum==0){
			lambda=0;
		}
		else{
			for(i=0;i<=n;i++){ 
				v[i]=1.0;
			}
			d=1;
			while(d>0.00001){//power 迭代法 找特徵值
				lambda_old=lambda; 
				for(i=1;i<=n;i++){
					Jv[i]=0;
					for(j=1;j<=n;j++){
						Jv[i]+=J[i][j]*v[j];
					}
				}
				norm=0;
				for(i=1;i<=n;i++){
					norm+=Jv[i]*Jv[i];
				}
				norm=sqrt(norm);
				for(i=1;i<=n;i++){
					v[i]=Jv[i]/norm;
				}
				for(i=1;i<=n;i++){
					Jv[i]=0;
					for(j=1;j<=n;j++){
						Jv[i]+=J[i][j]*v[j];
					}
				}
				lambda=0;
				for(i=1;i<=n;i++){
					lambda+=v[i]*Jv[i];
				}
				d=fabs(lambda-lambda_old);
			}
		}
		
		
		for(i=1;i<=n;i++){//重製rounds_max,again 
			for(j=1;j<=n;j++){
				rounds_max[i][j]=max;
				rounds_again[i][j][1]=rounds_max[i][j];
			}
		}
		
		for(i=1;i<=2*n;i++){//重製q 
			q[i]=0;
		}
		
		for(i=1;i<=n;i++){//重製E_max 
			E_max[i]=0;
		}
		
		for(k=2;k<=2*n;k++){//算q 
			if(k-2>0){
				for(i=1;i<=k-2;i++){
					for(j=1;j<=i;j++){
						q[k]+=P1_true[j]*P1_true[i+1-j];
					}
				}
			}
			for(i=1;i<=k-1;i++){
				q[k]+=P1_true[i]*P1_true[k-i]/2;
			}
		}
		
		
		for(rank=n;rank>=1;rank--){//找策略 
			for(b[1]=0;b[1]<=1;b[1]++){
				for(b[2]=0;b[2]<=1;b[2]++){
					for(b[3]=0;b[3]<=1;b[3]++){
						for(b[4]=0;b[4]<=1;b[4]++){
							for(b[5]=0;b[5]<=1;b[5]++){
									E_num=0;
									E_den=0;
									E=0;
									for(j=1;j<=n;j++){
										E_num+=(double)(1+rounds_again[rank][j][b[j]])*P1_true[j]*q[rank+j];
										E_den+=(double)(1+rounds_again[rank][j][b[j]])*P1_true[j];
										E=(double)E_num/E_den;
									}
									if(E>E_max[rank]){
										E_max[rank]=E;
										for(k=1;k<=n;k++){//以高等級限制低等級 
											rounds_best[rank][k]=rounds_again[rank][k][b[k]];
											if(rank>k){
												rounds_max[k][rank]=rounds_again[rank][k][b[k]];
												rounds_again[k][rank][1]=rounds_max[k][rank];
											}
										}								
									}
									
							}
						}
					}
				}
			}
		}
		if(fabs(E_max[3]-0.5)<0.00001&&rounds_best[3][5]==0&&rounds_best[3][4]==0){
			rounds_best[3][3]=max;
		}
		printf("第%d次策略\n",count+1);
		if(different==0){
			printf("多組p'皆收斂到相同P*上\n");
		}
		else{
			printf("並非任意p'皆收斂到相同P*上\n");
			break;
		}
		printf("由上次策略決定的新的遇到各等級機率P*\n");
		for(i=1;i<=n;i++){
			printf("等級%d:\t%.10f\n",i,P1_true[i]);
		}
		printf("上次策略在不動點的雅可比矩陣:\n"); 
		for(i=1;i<=n;i++){//雅可比矩陣 
			for(j=1;j<=n;j++){
				printf("%.3f\t",J[i][j]);
			}
			printf("\n");
		}
		printf("譜半徑:%.3f\n",fabs(lambda));
		printf("橫的是隊友等級；直的是自身等級\n");//顯示各等級再來一場最佳數 
		for(i=1;i<=n;i++){
			printf("\t%d",i);
		}
		printf("\n");
		for(i=1;i<=n;i++){
			printf("%d\t",i);
			for(j=1;j<=n;j++){
				printf("%d\t",rounds_best[i][j]);
			}
			printf("\n");
		}
		printf("各等級E:\n");
		for(i=1;i<=n;i++){
			printf("等級%d:%.10f\n",i,E_max[i]);
		}
		printf("\n\n\n");
		
		count+=1; 
	}
	return 0;
	
}

