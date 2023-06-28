NAME=Charu3_$(VERSION)

dist:
	rm -f $(NAME).zip
	mkdir $(NAME)
	cp -r Release/Charu3.exe _locale *.bmp *.wav $(NAME)
	zip -r $(NAME).zip $(NAME)
