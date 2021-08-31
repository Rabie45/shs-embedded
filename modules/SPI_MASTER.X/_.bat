@echo Cleaning..
@rmdir /s /q out
@mkdir out
@echo Compiling..
@xc8 --chip=16f877 newmain1.c --outdir=out
@ren && @xc8 --chip=16f877 slave.c --outdir=out