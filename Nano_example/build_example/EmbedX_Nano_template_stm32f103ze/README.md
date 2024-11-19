# EmbedX 

EmbedX 工程编译示例Demo。TDC264D 工程见project->AURIX

# 文件结构

- [x] app    ->应用程序
- [x] EmbedX_Nano    ->EmbedX_Nano框架
- [x] platform ->平台相关代码 如win32 、linux、MCU 
- [x] project 工程文件 -> ARM-GCC \KEIL \ EIDE\IAR \AURIX等

# 多编译工具支持

- stm32f103zet6编译示例 ，支持arm-gcc 支持keil ，eide。

- [x] ARM-GCC

- [x] KEIL

- [x] EIDE

- [ ] IAR

- TDC264D 示例仅支持AURIX Toolchain
# 多芯片支持

- [x] stm32f103zet6

- [x] TDC264D - TDC264D工程文件目前全部独立放在project->AURIX文件中


# 调试器支持
arm-gcc 调试 通过`.vscode` 文件中的 `launch.json`配设openocd支持ST-LINK在线调试。

```json
{
    // 使用 IntelliSense 了解相关属性。 
    // 悬停以查看现有属性的描述。
    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "cwd": "${workspaceRoot}",
            "executable": "./build/project/GCC/output/Template.elf",
            "name": "Debug with OpenOCD",
            "request": "launch",
            "type": "cortex-debug",
            "servertype": "openocd",
            "configFiles": [
                "interface/stlink-v2.cfg",
                "target/stm32f1x.cfg",
            ],
            "searchDir": [],
            "runToEntryPoint": "main",
            "showDevDebugOutput": "none",
            "svdFile" : "${config:STM32VSCodeExtension.cubeCLT.path}/STMicroelectronics_CMSIS_SVD/STM32F103.svd",//同时也可以在keil pack 对应芯片中找到对于芯片寄存器调试支持文件 .svd
        },
        {
            "name":"Build & Debug Microcontroller - ST-Link",
            "cwd":"${workspaceFolder}",
            "type":"cortex-debug",
            "executable": "${command:cmake.launchTargetPath}",
            "request": "launch",
            "servertype": "stlink",
            "device": "stm32f103zet6",
            "svdFile" : "${config:STM32VSCodeExtension.cubeCLT.path}/STMicroelectronics_CMSIS_SVD/STM32F103.svd", 
        }

    ]
}
```

- 其中svdFile文件可以再keil pack包中找到，可以支持调试时查看/修改寄存器

- 同理`stlink-v2.cfg`和`stm32f1x.cfg`也可以在pack包中找到，加入`cofigFiles`以支持在线调试

- cmake 编译需要指定编译elf文件 并在 "executable"中设置`"./build/project/GCC/output/Template.elf",`

- 同理eide调试需要指定eide编译出的elf文件。在vscode 的eide插件设置中勾选`EIDE.ARM.Option：Axf To Elf`，eide将会构建elf文件

![image-20241119220308471](./img/image-20241119220308471.png)