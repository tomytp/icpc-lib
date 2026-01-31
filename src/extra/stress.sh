P=a
make ${P} ${P}2 gen || exit 1
for ((i = 1; ; i++)) do
	./gen $i > tin
	./${P} < tin > tout_
	./${P}2 < tin > tout2
	if (! cmp -s tout tout2) then
		echo "--> entrada:"
		cat tin
		echo "--> saida1:"
		cat tout
		echo "--> saida2:"
		cat tout2
		break;
	fi
	echo $i
done
