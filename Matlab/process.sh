for file in non-face/*.png
do                       
 convert "$file" -colorspace Gray "$file"
 # do something on "$file"
 echo "$file"
done