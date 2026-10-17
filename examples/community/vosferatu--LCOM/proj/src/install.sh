cd ..
cp /home/lcom/proj/conf/duelnix /etc/system.conf.d
mkdir /home/duelnix
cp -vr res/ /home/duelnix
chmod 777 src/compile.sh
chmod 777 src/run.sh