function [ output_args ] = CR_Channel_Utilization(bs_count,curr_dev_id,xstr,ystr,yvalue,flag,h1,event_time)
%function for spectrum utilisation plot
%   Detailed explanation goes here
save CR_Channel_Utilization_log.mat
%xstr={ '52-54';'54-64';'64-74'}
%ystr={'inc1';'inc2';'inc3'}
%yvalue=[5;5;5];
figure(h1)
if(mod(flag,2)==0)
bsid = flag/2;
subplot(bs_count,2,flag)

 bar([yvalue],'yellow');set(gca,'XtickL',xstr,'YTick', []);
 
 for i = 1:length(xstr) %// Loop over each bar
     ypos = yvalue(i) + 0.1; %// Set y position, including gap
     htext = text(i,ypos,ystr{i});          %// Add text label
     set(htext,'VerticalAlignment','bottom', 'HorizontalAlignment','center','color','red')
 end
  xlabel('\bf SU frequency') 
title(['\bf Secondary Users - BS' num2str((flag)/2)])
axis([-1 length(xstr)+1 0 8]); %[L R T B]    

%CR_Cumulative_plot(bs_count,flag,xstr,h2) 
else
 bsid = (flag+1)/2;  
subplot(bs_count,2,flag)

bar([yvalue],'blue');set(gca,'XtickL',xstr,'YTick', []);

for i = 1:length(xstr) %// Loop over each bar
    ypos = yvalue(i) + 0.1; %// Set y position, including gap
    htext = text(i,ypos,ystr{i});          %// Add text label
    set(htext,'VerticalAlignment','bottom', 'HorizontalAlignment','center','color','red')
    
end
axis([0 length(xstr)+1 0 8]); %[L R T B]

xlabel('\bf PU frequency') 
title(['\bf Primary Users - BS' num2str((flag+1)/2)])

%CR_Cumulative_plot(bs_count,flag,xstr,h2)
end
figure(h1)
str=['AT TIME: ' num2str(event_time) ' microseconds'];
axes('Position',[0 0 1 1],'Visible','off');
h=text(0.45,0.95,'');
set(h, 'String', str);



for i = 1:length(xstr)
    if((yvalue(i)==5 && strncmp(ystr{i},'SU',2)))
    str=strcat('BS_',num2str(bsid),'_',ystr{i},'_',xstr{i},'.txt');
    fid = fopen(str,'a+');
    if(event_time>1)
    fprintf(fid,'%.0f',event_time);%/1000);
    fprintf(fid,'\r\n');
    end
    fclose('all');
    
    end
    str=char(ystr{i});
    if(strncmp(ystr{i},'PU',2)&& length(strfind(str,num2str(curr_dev_id))))
        str=strcat('BS_',num2str(bsid),'_',ystr{i},'_',xstr{i},'.txt');
    fid = fopen(str,'a+');
    if(event_time>1)
    fprintf(fid,'%.0f',event_time);%/1000);
    fprintf(fid,'\r\n');
    end
    fclose('all');
        
    end
end



end



