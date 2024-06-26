<div align="center">
    <h1>HyperOS</h1>
    <p>Simple microkernel written in x86 assembly and C</p>
    <p>
        <a href="https://github.com/HyperWinX/HyperOS/graphs/contributors">
            <img src="https://img.shields.io/github/contributors/HyperWinX/HyperOS" alt="contributors"/>
        </a>
        <a href="https://github.com/HyperWinX/HyperOS/commits/master">
            <img src="https://img.shields.io/github/last-commit/HyperWinX/HyperOS" alt="last commit"/>
        </a>
        <a href="https://github.com/HyperWinX/HyperOS/network/members">
            <img src="https://img.shields.io/github/forks/HyperWinX/HyperOS" alt="forks"/>
        </a>
        <a href="https://github.com/HyperWinX/HyperOS/stargazers">
            <img src="https://img.shields.io/github/stars/HyperWinX/HyperOS" alt="contributors"/>
        </a>
        <a href="https://github.com/HyperWinX/HyperOS/issues">
            <img src="https://img.shields.io/github/issues/HyperWinX/HyperOS" alt="contributors"/>
        </a>
    </p>
</div>
<br/>

# About HyperOS
This is small microkernel written primarily in C + Assembly.

### Currently implemented features
- Limine bootloader and LBP usage
- 64 bit mode (default if using Limine)

### Used projects
- [Limine terminal](https://github.com/ilobilo/limine-terminal.git)
    - [Flanterm](https://github.com/ilobilo/limine-terminal.git) (Limine terminal is based on Flanterm)

### Roadmap
- [ ] ~~Finish multiboot2 and extract all information possible~~
- [x] Move to Limine bootloader and use LBP to get information
- [ ] Implement allocator
- [ ] Basic FAT32 filesystem