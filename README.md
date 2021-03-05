# LoRa_stm8l

## LoRa技术

&ensp; LoRa采用星型网络结构，与网状网络结构相比，它是具有低延迟、最简单的网络结构。在实际应用中，采用LoRa协议的物联网设备无线通信距离超过15公里（郊区环境），电池使用寿命可达10年以上，并且能够将数百万的无线传感器节点与LoRa技术网关连接起来，这一优势是传统网络通讯标准无法达到的。常用的无线通信方式的传输距离、速率、功耗对比如下：

| 模式 | 最远传输距离 | 最高传输速率 | 最低接收功耗 |
| :----: | :----: | :----: | :----: |
| Bluetooth | 15m | 2MB/s | 6mA |
| WiFi | 100m | 54MB/s | 105mA |
| ZigBee | 75m | 250KB/s | 2mA |
| LoRa | 15km | 600KB/s | 3mA |

## SX1278芯片

&ensp; SX1278收发器主要采用LoRa远程调制解调器，用于超长距离扩频通信，抗干扰性强，能够最大限度降低电流功耗。借助Semtech的LoRa专利调制技术，SX1278采用低成本的晶体和物料即可获得超过-148dBm的高灵敏度。  
&ensp; 本设计中采用的是安信可的Ra-02模块，相关的技术参数可以参考<https://docs.ai-thinker.com/lora>。

## stm8l芯片

&ensp; 在本设计中，采用的是STM8L051F3处理器，相关的技术参数可以在<https://www.st.com/en/microcontrollers-microprocessors/stm8l051f3.html>上找到。STM8L051F3有 5种低功耗模式，分别为 Wait mode、Low-power run mode、Low-power wait mode、Active-halt mode和Halt mode。  
1. Wait mode：CPU的时钟停止工作，但是被选定的模块继续工作。内部中断、外部中断或者复位可以让CPU退出Wait模式。
2. Low-power run mode：CPU和所选外设正常运行，采用低速振荡器(LSI或 LSE)。单片机通过软件进入低功耗运行模式，并可以通过软件或者复位退出该模式。
3. Low-power wait mode：在低功耗下执行等待事件时进入该模式。类似于低功耗运行模式，但是CPU时钟停止。可以由复位、内部或触发外部事件(外部事件由定时器、串行接口、DMA产生)控制器(DMA1)和I/O端口退出该模式。当由事件触发唤醒时，系统回到低功耗运行模式。
4. Active-halt mode：除RTC外，CPU和外围时钟停止。可由RTC中断、外部中断或复位唤醒。
5. Halt mode: CPU和外围时钟停止，设备保持开机状态。RAM内容被保存。由外部中断或复位唤醒。一些外围设备也具有从停机唤醒功能。可在5μs内唤醒，唤醒时间很快。

在这5个不同的模式下运行，工作的电流如下表所示：
|  模式  |  电流  |
| :----: | :----:|
| Wait mode |  |
| Low-power run mode | 5.1μA |
| Low-power wait mode | 3μA |
| Active-halt mode | 1.3μA |
| Halt mode | 350nA |

&ensp; STM8L051F3单片机I/O引脚的最低驱动电流可以达到5μA，这些都是为了实现单片机的低功耗。

## 节点设计


## 网关设计


