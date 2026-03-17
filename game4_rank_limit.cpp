#include <stdio.h>
#include <math.h>
int main(){
	int n=5;
	int i,j,k;
	int b[n+5];
	int num,den;
	double t[2*n+1]={0};
	double P[2*n]={0};
	int rank;
	int rounds_max[n+1][n+1];//單場最大再來一場數 
	int rounds_best[n+1][n+1];//最佳再來一場數 
	int rounds_again[n+1][n+1][2];//迴圈跑的再來一場數(0和rounds_max) 
	int max=1;//初始最大再來一場數 
	double E=0;
	double E_num=0;
	double E_den=0;
	double E_max[n+1]={0};
	
	for(i=1;i<=n;i++){ 
		for(j=1;j<=n;j++){
			rounds_again[i][j][0]=0;
		}
	}
	while(1){
		max=1;//刷新條件 
		for(i=1;i<=2*n;i++){//重製t 
			t[i]=0;
		}
		
		for(i=1;i<=n;i++){
			printf("等級%d出現機率(分子、分母)\n",i);
			scanf(" %d %d",&num,&den);
			P[i]=(double)num/den;
		}
		
		for(k=2;k<=2*n;k++){//算t 
			if(k-2>0){
				for(i=1;i<=k-2;i++){
					for(j=1;j<=i;j++){
						t[k]+=P[j]*P[i+1-j];
					}
				}
			}
			for(i=1;i<=k-1;i++){
				t[k]+=P[i]*P[k-i]/2;
			}
		}
		while(max!=0){
			for(i=1;i<=n;i++){
				E_max[i]=0;
			}
			
			printf("最大再來一場場數?(按0重製機率)\n");
			scanf("%d",&max);
			for(i=1;i<=n;i++){
				for(j=1;j<=n;j++){
					rounds_max[i][j]=max;
					rounds_again[i][j][1]=rounds_max[i][j];
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
											E_num+=(double)(1+rounds_again[rank][j][b[j]])*P[j]*t[rank+j];
											E_den+=(double)(1+rounds_again[rank][j][b[j]])*P[j];
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
			
			printf("各等級最大E\n");//顯示各等級最佳E
			for(i=1;i<=n;i++){
					printf("%d\t%.10f\n",i,E_max[i]);
			}
		}
	}
	return 0;
	
}

