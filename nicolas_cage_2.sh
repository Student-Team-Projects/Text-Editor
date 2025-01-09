
for a in $(ls /usr/include/glib-2.0/glib/); do
    echo "#include <glib/$a>" 
done
