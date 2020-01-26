
Allocating common symbols
Common symbol       size              file

g_cursor_x          0x4               crt0.o
stdout              0x4               crt0.o
mm_prev_pointer     0x4               stdlib.o
mmblockList         0x400             stdlib.o
__libc_tty_id       0x4               crt0.o
last_valid          0x4               stdlib.o
heapList            0x400             stdlib.o
libcHeap            0x4               stdlib.o
errno               0x4               unistd.o
g_char_attrib       0x4               crt0.o
g_rows              0x4               crt0.o
Streams             0x80              crt0.o
optarg              0x4               unistd.o
g_using_gui         0x4               crt0.o
opterr              0x4               unistd.o
randseed            0x4               stdlib.o
heap_start          0x4               stdlib.o
prompt_out          0x400             crt0.o
g_available_heap    0x4               stdlib.o
g_heap_pointer      0x4               stdlib.o
g_columns           0x4               crt0.o
HEAP_SIZE           0x4               stdlib.o
optind              0x4               unistd.o
mmblockCount        0x4               stdlib.o
prompt_pos          0x4               crt0.o
stdin               0x4               crt0.o
last_size           0x4               stdlib.o
__Hostname_buffer   0x40              unistd.o
prompt_status       0x4               crt0.o
__Login_buffer      0x40              unistd.o
prompt_err          0x400             crt0.o
heap_end            0x4               stdlib.o
stderr              0x4               crt0.o
optopt              0x4               unistd.o
prompt              0x400             crt0.o
HEAP_END            0x4               stdlib.o
g_cursor_y          0x4               crt0.o
Heap                0x4               stdlib.o
prompt_max          0x4               crt0.o
current_mmblock     0x4               stdlib.o
heapCount           0x4               stdlib.o
HEAP_START          0x4               stdlib.o
__buffer            0x400             main.o

Memory Configuration

Name             Origin             Length             Attributes
*default*        0x0000000000000000 0xffffffffffffffff

Linker script and memory map


.text           0x0000000000401000     0x6000
                0x0000000000401000                code = .
                0x0000000000401000                _code = .
                0x0000000000401000                __code = .
 *(.head_x86)
 *(.text)
 .text          0x0000000000401000       0xe8 crt0.o
                0x0000000000401000                crt0
 .text          0x00000000004010e8       0x59 main.o
                0x00000000004010e8                cleanPrompt
                0x0000000000401137                main
 .text          0x0000000000401141        0x0 ctype.o
 .text          0x0000000000401141     0x359c stdio.o
                0x0000000000401189                stdio_atoi
                0x0000000000401250                stdio_fntos
                0x000000000040137a                remove
                0x0000000000401384                fclose
                0x00000000004013b3                fopen
                0x00000000004013e0                scroll
                0x00000000004014ad                puts
                0x00000000004014c8                clearerr
                0x00000000004014ea                fread
                0x000000000040158a                fwrite
                0x0000000000401594                prompt_putchar
                0x00000000004015e4                prompt_put_string
                0x0000000000401611                prompt_strcat
                0x000000000040163e                prompt_flush
                0x00000000004016a2                prompt_clean
                0x0000000000401ab2                printf3
                0x0000000000401acf                printf_atoi
                0x0000000000401bc0                printf_i2hex
                0x0000000000401c22                printf2
                0x0000000000401da7                stdio_nextline
                0x0000000000401de5                nlsprintf
                0x0000000000401e23                sprintf
                0x0000000000401e78                putchar
                0x0000000000401e94                libc_set_output_mode
                0x0000000000401ed8                outbyte
                0x0000000000402096                _outbyte
                0x00000000004020c5                input
                0x0000000000402222                getchar
                0x0000000000402250                fflush
                0x000000000040225a                fprintf
                0x000000000040228d                fputs
                0x00000000004022dd                nputs
                0x0000000000402318                __gets
                0x0000000000402349                fgets
                0x00000000004023cd                gets
                0x000000000040245c                ungetc
                0x00000000004024b7                ftell
                0x00000000004024c1                fileno
                0x00000000004024db                __gramado__getc
                0x0000000000402586                fgetc
                0x00000000004025a9                getc
                0x00000000004025bf                putc
                0x00000000004025d8                feof
                0x00000000004025f0                ferror
                0x0000000000402608                fseek
                0x0000000000402612                __gramado__putc
                0x00000000004026bd                debug_print
                0x00000000004026df                __serenity_fflush
                0x0000000000402780                __serenity_fputc
                0x0000000000402825                __serenity_putc
                0x000000000040283e                fputc
                0x0000000000402864                stdioSetCursor
                0x000000000040287f                stdioGetCursorX
                0x000000000040289a                stdioGetCursorY
                0x00000000004028b5                scanf
                0x0000000000402a56                sscanf
                0x0000000000402c11                kvprintf
                0x0000000000403a7b                printf
                0x0000000000403aa9                printf_draw
                0x0000000000403af1                vfprintf
                0x0000000000403b69                vprintf
                0x0000000000403b98                stdout_printf
                0x0000000000403bd4                stderr_printf
                0x0000000000403c10                perror
                0x0000000000403c27                rewind
                0x0000000000403c2d                snprintf
                0x0000000000403c41                stdio_initialize_standard_streams
                0x0000000000403c6c                libcStartTerminal
                0x0000000000403ce0                setbuf
                0x0000000000403ce6                setbuffer
                0x0000000000403d10                setlinebuf
                0x0000000000403d16                setvbuf
                0x0000000000403d98                filesize
                0x0000000000403dd8                fileread
                0x0000000000403e23                dprintf
                0x0000000000403e2d                vdprintf
                0x00000000004040b6                Wirzenius_Torvalds_vsprintf
                0x000000000040448b                Torvalds_printf
                0x00000000004044cb                vsnprintf
                0x00000000004044d5                vscanf
                0x00000000004044df                vsscanf
                0x00000000004044e9                vfscanf
                0x0000000000404500                tmpnam
                0x000000000040450a                tmpnam_r
                0x0000000000404514                tempnam
                0x000000000040451e                tmpfile
                0x0000000000404528                fdopen
                0x0000000000404532                freopen
                0x0000000000404549                open_memstream
                0x0000000000404553                open_wmemstream
                0x000000000040455d                fmemopen
                0x0000000000404567                fgetpos
                0x000000000040459e                fsetpos
                0x00000000004045b5                fpurge
                0x00000000004045cc                __fpurge
                0x00000000004045d5                ctermid
                0x00000000004045df                stdioInitialize
 .text          0x00000000004046dd       0x92 fcntl.o
                0x00000000004046dd                fcntl
                0x00000000004046e7                openat
                0x0000000000404722                open
                0x0000000000404748                creat
 .text          0x000000000040476f     0x1118 stdlib.o
                0x000000000040478c                rtGetHeapStart
                0x0000000000404796                rtGetHeapEnd
                0x00000000004047a0                rtGetHeapPointer
                0x00000000004047aa                rtGetAvailableHeap
                0x00000000004047b4                heapSetLibcHeap
                0x0000000000404867                heapAllocateMemory
                0x0000000000404a99                FreeHeap
                0x0000000000404aa3                heapInit
                0x0000000000404c36                stdlibInitMM
                0x0000000000404c99                libcInitRT
                0x0000000000404cc2                mktemp
                0x0000000000404ccc                rand
                0x0000000000404ce9                srand
                0x0000000000404cf7                xmalloc
                0x0000000000404d29                stdlib_die
                0x0000000000404d5f                malloc
                0x0000000000404d9b                realloc
                0x0000000000404dd8                free
                0x0000000000404dde                calloc
                0x0000000000404e24                zmalloc
                0x0000000000404e60                system
                0x0000000000405224                stdlib_strncmp
                0x0000000000405287                __findenv
                0x0000000000405352                getenv
                0x000000000040537f                setenv
                0x0000000000405389                unsetenv
                0x0000000000405393                atoi
                0x000000000040545a                reverse
                0x00000000004054c2                itoa
                0x0000000000405570                abs
                0x0000000000405580                strtod
                0x00000000004057b1                strtof
                0x00000000004057cd                strtold
                0x00000000004057e0                atof
                0x00000000004057f2                labs
                0x0000000000405802                mkstemp
                0x000000000040580c                mkostemp
                0x0000000000405816                mkstemps
                0x0000000000405820                mkostemps
                0x000000000040582a                ptsname
                0x0000000000405834                ptsname_r
                0x000000000040583e                posix_openpt
                0x0000000000405859                grantpt
                0x0000000000405863                getpt
                0x000000000040586d                unlockpt
                0x0000000000405877                getprogname
                0x0000000000405881                setprogname
 .text          0x0000000000405887      0xc19 string.o
                0x00000000004058b2                strcoll
                0x00000000004058cb                memsetw
                0x00000000004058f7                memcmp
                0x000000000040595c                strdup
                0x00000000004059ae                strndup
                0x0000000000405a0f                strnchr
                0x0000000000405a48                strrchr
                0x0000000000405a83                strtoimax
                0x0000000000405a8d                strtoumax
                0x0000000000405a97                strcasecmp
                0x0000000000405aff                strncpy
                0x0000000000405b55                strcmp
                0x0000000000405bba                strncmp
                0x0000000000405c1d                memset
                0x0000000000405c64                memoryZeroMemory
                0x0000000000405c8b                memcpy
                0x0000000000405cc8                strcpy
                0x0000000000405cfc                strlcpy
                0x0000000000405d5b                strcat
                0x0000000000405d8a                strchrnul
                0x0000000000405daf                strlcat
                0x0000000000405e3f                strncat
                0x0000000000405ea1                bcopy
                0x0000000000405ece                bzero
                0x0000000000405eef                strlen
                0x0000000000405f1d                strnlen
                0x0000000000405f58                strpbrk
                0x0000000000405fa6                strsep
                0x0000000000406024                strreplace
                0x000000000040605f                strcspn
                0x00000000004060fe                strspn
                0x000000000040619d                strtok_r
                0x0000000000406284                strtok
                0x000000000040629c                strchr
                0x00000000004062c8                memmove
                0x0000000000406349                memscan
                0x000000000040637d                strstr
                0x00000000004063dd                rindex
                0x00000000004063f0                strxfrm
                0x0000000000406442                strerror
                0x000000000040644c                strerror_r
                0x0000000000406456                strsignal
                0x0000000000406460                strtoupper
 .text          0x00000000004064a0       0x33 time.o
                0x00000000004064a0                time
                0x00000000004064c9                gettimeofday
 .text          0x00000000004064d3      0x6b1 unistd.o
                0x00000000004064d3                execv
                0x00000000004064f1                execve
                0x0000000000406550                read_ttyList
                0x000000000040657e                write_ttyList
                0x00000000004065ac                read_VC
                0x00000000004065da                write_VC
                0x0000000000406608                read
                0x0000000000406636                write
                0x0000000000406664                exit
                0x0000000000406684                fast_fork
                0x00000000004066ac                fork
                0x00000000004066e2                sys_fork
                0x0000000000406718                setuid
                0x0000000000406733                getuid
                0x000000000040674e                geteuid
                0x0000000000406758                getpid
                0x0000000000406770                getppid
                0x0000000000406788                getgid
                0x00000000004067a3                dup
                0x00000000004067bd                dup2
                0x00000000004067d9                dup3
                0x00000000004067f7                getpriority
                0x0000000000406801                setpriority
                0x000000000040680b                nice
                0x0000000000406815                pause
                0x000000000040681f                mkdir
                0x0000000000406833                rmdir
                0x000000000040683d                link
                0x0000000000406847                unlink
                0x0000000000406851                mlock
                0x000000000040685b                munlock
                0x0000000000406865                mlockall
                0x000000000040686f                munlockall
                0x0000000000406879                sysconf
                0x0000000000406883                fsync
                0x000000000040688d                fdatasync
                0x0000000000406897                sync
                0x000000000040689d                syncfs
                0x00000000004068a7                close
                0x00000000004068c5                pipe
                0x00000000004068e6                fpathconf
                0x00000000004068f0                pathconf
                0x00000000004068fa                __gethostname
                0x0000000000406926                gethostname
                0x000000000040694d                sethostname
                0x000000000040696e                getlogin
                0x000000000040699a                setlogin
                0x00000000004069c0                getusername
                0x0000000000406a3d                setusername
                0x0000000000406ab4                ttyname
                0x0000000000406af1                ttyname_r
                0x0000000000406afb                isatty
                0x0000000000406b1e                getopt
                0x0000000000406b28                alarm
                0x0000000000406b32                brk
                0x0000000000406b3c                execvp
                0x0000000000406b46                execvpe
                0x0000000000406b50                chdir
                0x0000000000406b5a                fchdir
                0x0000000000406b64                sleep
                0x0000000000406b6e                _exit
 .text          0x0000000000406b84      0x1c2 termios.o
                0x0000000000406b84                tcgetpgrp
                0x0000000000406b8e                tcsetpgrp
                0x0000000000406b98                tcgetattr
                0x0000000000406bb6                tcsetattr
                0x0000000000406c2f                tcsendbreak
                0x0000000000406c39                tcdrain
                0x0000000000406c43                tcflush
                0x0000000000406c4d                tcflow
                0x0000000000406c57                cfmakeraw
                0x0000000000406cd1                cfgetispeed
                0x0000000000406ce9                cfgetospeed
                0x0000000000406d01                cfsetispeed
                0x0000000000406d18                cfsetospeed
                0x0000000000406d2f                cfsetspeed
 .text          0x0000000000406d46       0x3d ioctl.o
                0x0000000000406d46                ioctl
 .text          0x0000000000406d83       0x28 stubs.o
                0x0000000000406d83                gramado_system_call
                0x0000000000407000                . = ALIGN (0x1000)
 *fill*         0x0000000000406dab      0x255 

.iplt           0x0000000000407000        0x0
 .iplt          0x0000000000407000        0x0 crt0.o

.rodata         0x0000000000407000      0xc11
 .rodata        0x0000000000407000       0x22 crt0.o
 *fill*         0x0000000000407022       0x1e 
 .rodata        0x0000000000407040      0x100 ctype.o
                0x0000000000407040                _ctype
 .rodata        0x0000000000407140      0x524 stdio.o
                0x0000000000407380                hex2ascii_data
 *fill*         0x0000000000407664        0x4 
 .rodata        0x0000000000407668      0x520 stdlib.o
 .rodata        0x0000000000407b88       0x89 unistd.o

.eh_frame       0x0000000000407c14     0x2438
 .eh_frame      0x0000000000407c14       0x34 crt0.o
 .eh_frame      0x0000000000407c48       0x40 main.o
                                         0x58 (size before relaxing)
 .eh_frame      0x0000000000407c88      0xdc0 stdio.o
                                        0xdd8 (size before relaxing)
 .eh_frame      0x0000000000408a48       0x80 fcntl.o
                                         0x98 (size before relaxing)
 .eh_frame      0x0000000000408ac8      0x600 stdlib.o
                                        0x618 (size before relaxing)
 .eh_frame      0x00000000004090c8      0x5a0 string.o
                                        0x5b8 (size before relaxing)
 .eh_frame      0x0000000000409668       0x40 time.o
                                         0x58 (size before relaxing)
 .eh_frame      0x00000000004096a8      0x7a0 unistd.o
                                        0x7b8 (size before relaxing)
 .eh_frame      0x0000000000409e48      0x1c0 termios.o
                                        0x1d8 (size before relaxing)
 .eh_frame      0x000000000040a008       0x20 ioctl.o
                                         0x38 (size before relaxing)
 .eh_frame      0x000000000040a028       0x24 stubs.o
                                         0x3c (size before relaxing)

.rel.dyn        0x000000000040a04c        0x0
 .rel.got       0x000000000040a04c        0x0 crt0.o
 .rel.iplt      0x000000000040a04c        0x0 crt0.o
 .rel.text      0x000000000040a04c        0x0 crt0.o

.data           0x000000000040a050      0xfb0
                0x000000000040a050                data = .
                0x000000000040a050                _data = .
                0x000000000040a050                __data = .
 *(.data)
 .data          0x000000000040a050       0x14 crt0.o
 .data          0x000000000040a064       0x1b main.o
                0x000000000040a064                String
 .data          0x000000000040a07f        0x0 ctype.o
 .data          0x000000000040a07f        0x0 stdio.o
 .data          0x000000000040a07f        0x0 fcntl.o
 *fill*         0x000000000040a07f        0x1 
 .data          0x000000000040a080        0x8 stdlib.o
                0x000000000040a080                _infinity
 .data          0x000000000040a088        0x0 string.o
 .data          0x000000000040a088        0x0 time.o
 .data          0x000000000040a088        0x0 unistd.o
 .data          0x000000000040a088        0x0 termios.o
 .data          0x000000000040a088        0x0 ioctl.o
 .data          0x000000000040a088        0x0 stubs.o
                0x000000000040b000                . = ALIGN (0x1000)
 *fill*         0x000000000040a088      0xf78 

.got            0x000000000040b000        0x0
 .got           0x000000000040b000        0x0 crt0.o

.got.plt        0x000000000040b000        0x0
 .got.plt       0x000000000040b000        0x0 crt0.o

.igot.plt       0x000000000040b000        0x0
 .igot.plt      0x000000000040b000        0x0 crt0.o

.bss            0x000000000040b000     0xaa44
                0x000000000040b000                bss = .
                0x000000000040b000                _bss = .
                0x000000000040b000                __bss = .
 *(.bss)
 .bss           0x000000000040b000        0x0 crt0.o
 .bss           0x000000000040b000        0x0 main.o
 .bss           0x000000000040b000        0x0 ctype.o
 .bss           0x000000000040b000      0x421 stdio.o
 .bss           0x000000000040b421        0x0 fcntl.o
 *fill*         0x000000000040b421       0x1f 
 .bss           0x000000000040b440     0x8020 stdlib.o
                0x000000000040b440                environ
 .bss           0x0000000000413460        0x4 string.o
 .bss           0x0000000000413464        0x0 time.o
 *fill*         0x0000000000413464       0x1c 
 .bss           0x0000000000413480      0x19f unistd.o
                0x0000000000413480                __execv_environ
 .bss           0x000000000041361f        0x0 termios.o
 .bss           0x000000000041361f        0x0 ioctl.o
 .bss           0x000000000041361f        0x0 stubs.o
                0x0000000000414000                . = ALIGN (0x1000)
 *fill*         0x000000000041361f      0x9e1 
 COMMON         0x0000000000414000      0xd08 crt0.o
                0x0000000000414000                g_cursor_x
                0x0000000000414004                stdout
                0x0000000000414008                __libc_tty_id
                0x000000000041400c                g_char_attrib
                0x0000000000414010                g_rows
                0x0000000000414020                Streams
                0x00000000004140a0                g_using_gui
                0x00000000004140c0                prompt_out
                0x00000000004144c0                g_columns
                0x00000000004144c4                prompt_pos
                0x00000000004144c8                stdin
                0x00000000004144cc                prompt_status
                0x00000000004144e0                prompt_err
                0x00000000004148e0                stderr
                0x0000000000414900                prompt
                0x0000000000414d00                g_cursor_y
                0x0000000000414d04                prompt_max
 *fill*         0x0000000000414d08       0x18 
 COMMON         0x0000000000414d20      0x400 main.o
                0x0000000000414d20                __buffer
 COMMON         0x0000000000415120        0x0 stdio.o
 COMMON         0x0000000000415120      0x878 stdlib.o
                0x0000000000415120                mm_prev_pointer
                0x0000000000415140                mmblockList
                0x0000000000415540                last_valid
                0x0000000000415560                heapList
                0x0000000000415960                libcHeap
                0x0000000000415964                randseed
                0x0000000000415968                heap_start
                0x000000000041596c                g_available_heap
                0x0000000000415970                g_heap_pointer
                0x0000000000415974                HEAP_SIZE
                0x0000000000415978                mmblockCount
                0x000000000041597c                last_size
                0x0000000000415980                heap_end
                0x0000000000415984                HEAP_END
                0x0000000000415988                Heap
                0x000000000041598c                current_mmblock
                0x0000000000415990                heapCount
                0x0000000000415994                HEAP_START
 *fill*         0x0000000000415998        0x8 
 COMMON         0x00000000004159a0       0xa4 unistd.o
                0x00000000004159a0                errno
                0x00000000004159a4                optarg
                0x00000000004159a8                opterr
                0x00000000004159ac                optind
                0x00000000004159c0                __Hostname_buffer
                0x0000000000415a00                __Login_buffer
                0x0000000000415a40                optopt
                0x0000000000415a44                end = .
                0x0000000000415a44                _end = .
                0x0000000000415a44                __end = .
LOAD crt0.o
LOAD main.o
LOAD ctype.o
LOAD stdio.o
LOAD fcntl.o
LOAD stdlib.o
LOAD string.o
LOAD time.o
LOAD unistd.o
LOAD termios.o
LOAD ioctl.o
LOAD stubs.o
OUTPUT(FALSE.BIN elf32-i386)

.comment        0x0000000000000000       0x11
 .comment       0x0000000000000000       0x11 crt0.o
                                         0x12 (size before relaxing)
 .comment       0x0000000000000011       0x12 main.o
 .comment       0x0000000000000011       0x12 ctype.o
 .comment       0x0000000000000011       0x12 stdio.o
 .comment       0x0000000000000011       0x12 fcntl.o
 .comment       0x0000000000000011       0x12 stdlib.o
 .comment       0x0000000000000011       0x12 string.o
 .comment       0x0000000000000011       0x12 time.o
 .comment       0x0000000000000011       0x12 unistd.o
 .comment       0x0000000000000011       0x12 termios.o
 .comment       0x0000000000000011       0x12 ioctl.o
 .comment       0x0000000000000011       0x12 stubs.o

.note.GNU-stack
                0x0000000000000000        0x0
 .note.GNU-stack
                0x0000000000000000        0x0 crt0.o
 .note.GNU-stack
                0x0000000000000000        0x0 main.o
 .note.GNU-stack
                0x0000000000000000        0x0 ctype.o
 .note.GNU-stack
                0x0000000000000000        0x0 stdio.o
 .note.GNU-stack
                0x0000000000000000        0x0 fcntl.o
 .note.GNU-stack
                0x0000000000000000        0x0 stdlib.o
 .note.GNU-stack
                0x0000000000000000        0x0 string.o
 .note.GNU-stack
                0x0000000000000000        0x0 time.o
 .note.GNU-stack
                0x0000000000000000        0x0 unistd.o
 .note.GNU-stack
                0x0000000000000000        0x0 termios.o
 .note.GNU-stack
                0x0000000000000000        0x0 ioctl.o
 .note.GNU-stack
                0x0000000000000000        0x0 stubs.o
