#include "main.h"
#include "802_22.h"
#include "List.h"



int fn_NetSim_CR_Channel_Utilization_Init()
{
	int i=0,bsid=0,j=0,k=0,nLoop=0;
	int nDeviceId=0;
	NetSim_LINKS* pstruLink;

	sprintf(str,"cd 'C:\\Users\\Tetcos\\Desktop\\11Webinar - NetSim_Data_Analytics&Visualization\\Channel Utilization'");
	fn_matlab_execute_command(str);
	sprintf(str,"delete 'BS_*.txt'");
	fn_matlab_execute_command(str);
	for(i=0;i<NETWORK->nDeviceCount;i++)
	{
		if(NETWORK->ppstruDeviceList[i]->nDeviceType==BASESTATION)
		{
			bs_count++;
		}
	}
	sprintf(str,"bs_count=%d",bs_count);
	fn_matlab_execute_command(str);

	//sprintf(str,"bs_su_count=zeros(1,%d);",bs_count);
	//fn_matlab_execute_command(str);

	bs_id=(int*)calloc((bs_count),sizeof(int));
	bs_crcpe_count=(int*)calloc((bs_count),sizeof(int));
	SU_name=(char***)calloc(bs_count,sizeof(char));

	bs_crcpe_list=(int**)calloc(bs_count,sizeof(double));

	for(i=0;i<NETWORK->nDeviceCount;i++)
	{
		if(NETWORK->ppstruDeviceList[i]->nDeviceType==BASESTATION)
		{
			bs_id[j]=NETWORK->ppstruDeviceList[i]->nDeviceId;
			pstruLink = DEVICE_PHYLAYER(bs_id[j],1)->pstruNetSimLinks;
			for(nLoop=0;nLoop<pstruLink->puniDevList.pstrup2MP.nConnectedDeviceCount-1;nLoop++)
			{
				if(nDeviceId != pstruLink->puniDevList.pstrup2MP.anDevIds[nLoop])
				{
					bs_crcpe_count[j]++;
				}
			}

			bs_crcpe_list[j]=(int*)calloc(bs_crcpe_count[j],sizeof*(bs_crcpe_list[j]));
			j++;

		}


	}

	for(i=0;i<bs_count;i++)
	{
		pstruLink = DEVICE_PHYLAYER(bs_id[i],1)->pstruNetSimLinks;
		k=0;
		for(nLoop=0;nLoop<pstruLink->puniDevList.pstrup2MP.nConnectedDeviceCount-1;nLoop++)
		{
			if(nDeviceId != pstruLink->puniDevList.pstrup2MP.anDevIds[nLoop])
			{

				bs_crcpe_list[i][k]=pstruLink->puniDevList.pstrup2MP.anDevIds[nLoop];
				k++;

			}
		}




	}


	fn_matlab_execute_command("h1=figure('Name','CR- INSTANTANEOUS CHANNEL UTILIZATION PLOT','Position',[100 100 1000 500],'NumberTitle','off')");
	

	for(i=0;i<NETWORK->nDeviceCount;i++)
	{
		if(NETWORK->ppstruDeviceList[i]->nDeviceType==BASESTATION)
		{

			nDeviceId=NETWORK->ppstruDeviceList[i]->nDeviceId;
			bsid++;
			fn_NetSim_CR_Channel_Utilization_PU_Plot_Parameters(nDeviceId,bsid);
			fn_NetSim_CR_Channel_Utilization_SU_Plot_Parameters(nDeviceId,bsid);




		}
	}
}

int fn_NetSim_CR_Channel_Utilization_PU_Plot_Parameters(int nDeviceId, int plot_bs_id)
{
	int i=0,e_t=0;
	BS_MAC* pstruBSMAC = (BS_MAC*)DEVICE_MACVAR(nDeviceId,1);
	//In_ystr
	sprintf(str,"In_ystr%d={",plot_bs_id);
	for(i=1;i<=pstruBSMAC->nIncumbentCount;i++)
	{
		sprintf(str,"%s'PU%d'",str,i);
		if(i!=pstruBSMAC->nIncumbentCount)
			sprintf(str,"%s;",str);
	}
	sprintf(str,"%s}",str);
	fn_matlab_execute_command(str);

	//In_xstr
	sprintf(str,"");
	sprintf(str,"In_xstr%d={",plot_bs_id);
	for(i=1;i<=pstruBSMAC->nIncumbentCount;i++)
	{
		sprintf(str,"%s'%d-%d'",str,pstruBSMAC->pstruIncumbent[i-1]->nStartFrequeny,pstruBSMAC->pstruIncumbent[i-1]->nEndFrequency);
		if(i!=pstruBSMAC->nIncumbentCount)
			sprintf(str,"%s;",str);
	}
	sprintf(str,"%s}",str);
	fn_matlab_execute_command(str);


	//fn_NetSim_CR_Channel_Utilization_plot_update_PU_yvalue_Update(nDeviceId);
	sprintf(str,"");
	sprintf(str,"In_yvalue%d=[",plot_bs_id);
	for(i=1;i<=pstruBSMAC->nIncumbentCount;i++)
	{

	sprintf(str,"%s%d",str,0);
	if(i!=pstruBSMAC->nIncumbentCount)
	sprintf(str,"%s;",str);
	}
	sprintf(str,"%s]",str);
	fn_matlab_execute_command(str);
	

	fn_NetSim_CR_Channel_Utilization_BS_Subplot(nDeviceId);


	sprintf(str,"");
	e_t=pstruEventDetails->dEventTime;
	sprintf(str,"CR_Channel_Utilization(bs_count,%d,In_xstr%d,In_ystr%d,In_yvalue%d,%d,h1,%d)",0,plot_bs_id,plot_bs_id,plot_bs_id,plot_flag,e_t);
	fn_matlab_execute_command(str);
}

int fn_NetSim_CR_Channel_Utilization_BS_Subplot(int devid)
{
	int i=0;
	for(i=0;i<bs_count;i++)
	{
		if(devid==bs_id[i])
		{
			plot_flag=((i+1)*2)-1;
			break;
		}
	}
	return i+1;
}

int fn_NetSim_CR_Channel_Utilization_plot_update_PU_yvalue_Update(int devid,int Inc_id,int state)
{
	int i=0;

	BS_MAC* pstruBSMAC = (BS_MAC*)DEVICE_MACVAR(devid,1);
	devid=fn_NetSim_CR_Channel_Utilization_BS_Subplot(devid);
	sprintf(str,"");
	sprintf(str,"In_yvalue%d(%d)=%d",devid,Inc_id,state);
	
	fn_matlab_execute_command(str);
}

int fn_NetSim_CR_Channel_Utilization_SU_Plot_Parameters(int nDeviceId, int plot_bs_id)
{

	int nStartFrequency;
	int nEndFrequency;
	int nChannelCount;
	int nChannelBandwidth;
	int nLoop;
	double u_f=0,l_f=0,e_t;
	struct stru_802_22_Channel* pstruFirstChannel;
	BS_MAC* pstruBSMAC = (BS_MAC*)DEVICE_MACVAR(nDeviceId,1);
	BS_PHY* pstruBSPhy = (BS_PHY*)DEVICE_PHYVAR(nDeviceId,1);
	NETSIM_ID nCPEID=0;
	unsigned int nChannelNumber = 1;
	nStartFrequency = pstruBSPhy->nMINFrequency;
	nEndFrequency = pstruBSPhy->nMAXFrequency;
	nChannelBandwidth = pstruBSPhy->nChannelBandwidth;
	nChannelCount = (nEndFrequency-nStartFrequency)/nChannelBandwidth;

	SU_name[plot_bs_id-1]=(char*)calloc(nChannelCount,sizeof*(SU_name[nLoop]));
	pstruFirstChannel = (struct stru_802_22_Channel*)fnpAllocateMemory(nChannelCount+1,sizeof *pstruFirstChannel);

	
	pstruFirstChannel=pstruBSPhy->pstruChannelSet;

	//SU_xstr
	sprintf(str,"");
	sprintf(str,"SU_xstr%d={",plot_bs_id);
	for(nLoop=0;nLoop<nChannelCount;nLoop++)
	{
		l_f=pstruFirstChannel->dLowerFrequency;
		u_f=pstruFirstChannel->dUpperFrequency;
		sprintf(str,"%s'%.0lf-%.0lf'",str,l_f,u_f);
		if(nLoop!=nChannelCount-1)
			sprintf(str,"%s;",str);
		pstruFirstChannel++;
		SU_name[plot_bs_id-1][nLoop]=malloc(512);		
		sprintf(SU_name[plot_bs_id-1][nLoop],"");
	}
	sprintf(str,"%s}",str);
	fn_matlab_execute_command(str);

	//sprintf(str,"bs_su_count(%d)=%d;",plot_bs_id,nChannelCount);
	//fn_matlab_execute_command(str);

	//SU_yvalue
	//fn_NetSim_CR_Channel_Utilization_plot_update_SU_yvalue_Update(nDeviceId,plot_bs_id,0);
	sprintf(str,"");
	sprintf(str,"SU_yvalue%d=[",plot_bs_id);
	for(nLoop=0;nLoop<nChannelCount;nLoop++)
	{

	sprintf(str,"%s%d",str,0);
	if(nLoop!=nChannelCount-1)
	sprintf(str,"%s;",str);
	}
	sprintf(str,"%s]",str);
	fn_matlab_execute_command(str);

	//SU_ystr

	fn_NetSim_CR_Channel_Utilization_SU_ystr_Update(nDeviceId,plot_bs_id);
	/*sprintf(str,"");
	sprintf(str,"SU_ystr%d={",plot_bs_id);


	for(nLoop=0;nLoop<nChannelCount;nLoop++)
	{	
	sprintf(str,"%s%s",str,SU_name[nLoop]);
	if(nLoop!=nChannelCount-1)
	sprintf(str,"%s;",str);





	}
	sprintf(str,"%s}",str);
	fn_matlab_execute_command(str);		*/


	fn_NetSim_CR_Channel_Utilization_BS_Subplot(nDeviceId);

	sprintf(str,"");
	e_t=pstruEventDetails->dEventTime;
	sprintf(str,"CR_Channel_Utilization(bs_count,%d,SU_xstr%d,SU_ystr%d,SU_yvalue%d,%d,h1,%d)",0,plot_bs_id,plot_bs_id,plot_bs_id,plot_flag+1,e_t);
	fn_matlab_execute_command(str);





}

int fn_NetSim_CR_Channel_Utilization_SU_ystr_Update(int nDeviceId, int plot_bs_id)
{
	int nStartFrequency;
	int nEndFrequency;
	int nChannelCount;
	int nChannelBandwidth;
	int nLoop;
	BS_PHY* pstruBSPhy = (BS_PHY*)DEVICE_PHYVAR(nDeviceId,1);
	nStartFrequency = pstruBSPhy->nMINFrequency;
	nEndFrequency = pstruBSPhy->nMAXFrequency;
	nChannelBandwidth = pstruBSPhy->nChannelBandwidth;
	nChannelCount = (nEndFrequency-nStartFrequency)/nChannelBandwidth;

	//SU_ystr

	sprintf(str,"");
	sprintf(str,"SU_ystr%d={",plot_bs_id);
	for(nLoop=0;nLoop<nChannelCount;nLoop++)
	{	
		sprintf(str,"%s'%s'",str,SU_name[plot_bs_id-1][nLoop]);
		if(nLoop!=nChannelCount-1)
			sprintf(str,"%s;",str);

	}

	sprintf(str,"%s}",str);
	fn_matlab_execute_command(str);		
}

int fn_NetSim_CR_Channel_Utilization_plot_update_SU_yvalue_Update(int nDeviceId,int plot_bs_id,int ch_num,int ch_state)
{

	int nStartFrequency;
	int nEndFrequency;
	int nChannelCount;
	int nChannelBandwidth;
	int nLoop;
	BS_PHY* pstruBSPhy = (BS_PHY*)DEVICE_PHYVAR(nDeviceId,1);
	BS_MAC* pstruBSMAC = (BS_MAC*)DEVICE_MACVAR(nDeviceId,1);
	nStartFrequency = pstruBSPhy->nMINFrequency;
	nEndFrequency = pstruBSPhy->nMAXFrequency;
	nChannelBandwidth = pstruBSPhy->nChannelBandwidth;
	nChannelCount = (nEndFrequency-nStartFrequency)/nChannelBandwidth;

	//SU_yvalue


	
	sprintf(str,"");



	
	for(nLoop=0;nLoop<nChannelCount;nLoop++)
	{
		if(nLoop+1==ch_num)
		{
			sprintf(str,"SU_yvalue%d(%d)=%d",plot_bs_id,ch_num,ch_state);
		}
				
	}
	
	fn_matlab_execute_command(str);
}