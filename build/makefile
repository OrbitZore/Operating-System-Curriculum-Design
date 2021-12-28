all : all.pdf final.pdf
all.pdf : all.md
	pandoc -s \
		--template eisvogel \
		--listings \
		-H ../header/header.tex \
		--pdf-engine=xelatex \
		all.md \
		-o all.pdf

final.pdf : all.pdf ../header/infoheader.pdf
	pdfunite ../header/infoheader.pdf all.pdf final.pdf

clean : 
	rm -f final.pdf all.pdf