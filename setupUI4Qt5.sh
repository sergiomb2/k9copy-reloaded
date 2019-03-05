for UI in `ls */*/*.ui`; 
do 
sed -i 's,kpushbutton.h,qpushbutton.h,g' $UI; 
sed -i 's,KPushButton,QPushButton,g' $UI; 


done
