建议设备驱动层使用以下接口来适配该文件
其中
*handle为iic句柄

dev_adress为设备7位地址，不需要进行左移和读写位操作

reg_adress为要读取或者写入的寄存器地址

*data为要写入的数据或存储读取数据的指针

len为要读取的寄存器个数

us为延时的时间，单位为微秒。

typedef struct {
    void (*write_reg)(void *handle,uint8_t dev_adress,uint8_t reg_adress,uint8_t *data,uint8_t len);
    uint8_t (*read_reg)(void *handle,uint8_t dev_address,uint8_t reg_address,uint8_t *data,uint8_t len);
    void (*mdelay)(uint32_t us);
    void *handle;  // 用于存储特定于平台的上下文信息
} bsp_设备名_t;


初始化接口示例
bsp_设备名_t dev= {
        .write_reg = WriteRegister,
        .read_reg = ReadRegister,
        .mdelay = HAL_Delay_us,
        .handle = NULL,  // 如果不需要特定于平台的上下文信息，则为NULL
    };