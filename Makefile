all:
	python3 scripts/os.py build
run: all
	qemu-system-i386 -cdrom dist/hyperos.iso -monitor stdio -vnc :1 -machine type=pc-i440fx-3.1
run_debug:
	python3 scripts/os.py run_debug
clean:
	rm -rf build dist public *.gcno a.out
shell:
	@sudo docker-compose run dev