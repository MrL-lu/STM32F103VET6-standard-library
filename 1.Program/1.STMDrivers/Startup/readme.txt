启动文件 区别
startup_stm32f10x_ld.s ld: low-density 小容量， FLASH 容量在 16-32K 之间
startup_stm32f10x_md.s md: medium-density 中容量，FLASH 容量在 64-128K 之间
startup_stm32f10x_hd.s hd: high-density 中容量， FLASH 容量在 256-512K 之间
startup_stm32f10x_xl.s xl: 超大容量， FLASH 容量在 512-1024K 之间
以上四种都属于基本型，包括 STM32F101xx、STM32F102xx、STM32F103xx 系列
startup_stm32f10x_cl.s cl:connectivity line devices 互联型，特指 STM32F105xx 和
STM32F107xx 系列
startup_stm32f10x_ld_vl.s
startup_stm32f10x_md_vl.s vl:value line devices 超值型系列，特指 STM32F100xx 系列
startup_stm32f10x_hd_vl.s