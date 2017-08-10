function [ output_args ] = CR_Cumulative_plot( bs_count,flag,xstr,h2,event_time)

figure(h2)
if(mod(flag,2)==0)
bsid = flag/2;
subplot(bs_count,2,flag)


file_Str=strcat('BS_',num2str(bsid),'*.txt');
log_list=dir(file_Str);
	for j=1:length(log_list)
	tmp=log_list(j).name
	parts=strread(tmp,'%s','delimiter','_,.');
    yvalue=zeros(1,length(xstr));
    %barh([yvalue]);set(gca,'XtickL',[],'YTickL',xstr);
    barh([yvalue]);set(gca,'YTickL',xstr);
    if(strncmp(parts(3),'SU',2))
        for k=1:length(xstr)
            if strcmp(xstr(k),parts(4))
            fid=fopen(log_list(j).name);
            mx=fscanf(fid,'%f');
            fclose('all');
            hold on
            plot(mx,k,'b.','MarkerSize',12)
            hold on
                
            end
        end
    end
   % bar([yvalue],'yellow');set(gca,'XtickL',xstr,'YTick', []);
   % barh([yvalue],'yellow');set(gca,'XtickL',[],'YTickL',xstr);
	
    end
     xlabel('\bf Simulation Time(microsecond)') 
     title(['\bf Secondary Users - BS ' num2str((flag)/2)])


else
    
bsid = (flag+1)/2;  
subplot(bs_count,2,flag)


file_Str=strcat('BS_',num2str(bsid),'*.txt');
log_list=dir(file_Str);
	for j=1:length(log_list)
        tmp=log_list(j).name;
        parts=strread(tmp,'%s','delimiter','_,.');
        yvalue=zeros(1,length(xstr));
        %barh([yvalue]);set(gca,'XtickL',[],'YTickL',xstr);
        barh(yvalue);set(gca,'YTickL',xstr);
        if(strncmp(parts(3),'PU',2))
            for k=1:length(xstr)
                if strcmp(xstr(k),parts(4))
                fid=fopen(log_list(j).name);
                mx=fscanf(fid,'%f');
                fclose('all');
                hold on
                plot(mx,k,'r.','MarkerSize',12)
                hold on

                    for l=1:(length(mx))
                        if(mod(l,2)~=0 && l~=length(mx))
                            if(mx(l)~=mx(l+1))
                                line([mx(l) mx(l+1)],[k k],'Color','red','LineWidth',2.5);
                            else
                                if(l==1)
                                line([0 mx(l)],[k k],'Color','red','LineWidth',2.5);
                                end
                                if(l<=length(mx)-2)
                                line([mx(l+1) mx(l+2)],[k k],'Color','red','LineWidth',2.5);
                                end
                            end
                        
                         
                        end
                        
                    end
                end
            end
        end
       % bar([yvalue],'yellow');set(gca,'XtickL',xstr,'YTick', []);
       % barh([yvalue],'yellow');set(gca,'XtickL',[],'YTickL',xstr);

  end
        xlabel('\bf Simulation Time(microsecond)') 
        title(['\bf Primary Users - BS ' num2str((flag+1)/2)])
        
%         PU=line([0 0],[0 0],'color','red');
%         SU=line([0 0],[0 0],'color','blue');
%         legend([PU,SU],'PU','SU');

 end
     

end

