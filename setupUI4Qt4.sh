for UI in `ls */*/*.ui`; 
do 
sed -i 's,qpushbutton.h,kpushbutton.h,g' $UI; 
sed -i 's,QPushButton,KPushButton,g' $UI;

done
