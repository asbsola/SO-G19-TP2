for i in {0..255}
do
    c=`echo $i | awk '{ printf("%c", $0); }'`
    convert -resize 8x16\! -font Nimbus-Mono-L -pointsize 10 label:$c xbm:- >> font.xbm
done
