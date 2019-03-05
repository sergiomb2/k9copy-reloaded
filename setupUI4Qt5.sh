for UI in `ls */*/*.ui`; 
do 
sed -i 's,kpushbutton.h,qpushbutton.h,g;s,KPushButton,QPushButton,g' $UI; 
done
