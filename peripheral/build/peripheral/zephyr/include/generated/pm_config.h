/* File generated by C:\ncs\v2.6.1\nrf\scripts\partition_manager_output.py, do not modify */
#ifndef PM_CONFIG_H__
#define PM_CONFIG_H__
#define PM_APP_OFFSET 0x0
#define PM_APP_ADDRESS 0x0
#define PM_APP_END_ADDRESS 0x7e000
#define PM_APP_SIZE 0x7e000
#define PM_APP_NAME app
#define PM_APP_ID 0
#define PM_app_ID PM_APP_ID
#define PM_app_IS_ENABLED 1
#define PM_0_LABEL APP
#define PM_APP_DEV flash_controller
#define PM_APP_DEFAULT_DRIVER_KCONFIG CONFIG_SOC_FLASH_NRF
#define PM_SETTINGS_STORAGE_OFFSET 0x7e000
#define PM_SETTINGS_STORAGE_ADDRESS 0x7e000
#define PM_SETTINGS_STORAGE_END_ADDRESS 0x80000
#define PM_SETTINGS_STORAGE_SIZE 0x2000
#define PM_SETTINGS_STORAGE_NAME settings_storage
#define PM_SETTINGS_STORAGE_ID 1
#define PM_settings_storage_ID PM_SETTINGS_STORAGE_ID
#define PM_settings_storage_IS_ENABLED 1
#define PM_1_LABEL SETTINGS_STORAGE
#define PM_SETTINGS_STORAGE_DEV flash_controller
#define PM_SETTINGS_STORAGE_DEFAULT_DRIVER_KCONFIG CONFIG_SOC_FLASH_NRF
#define PM_SRAM_PRIMARY_OFFSET 0x0
#define PM_SRAM_PRIMARY_ADDRESS 0x20000000
#define PM_SRAM_PRIMARY_END_ADDRESS 0x20020000
#define PM_SRAM_PRIMARY_SIZE 0x20000
#define PM_SRAM_PRIMARY_NAME sram_primary
#define PM_NUM 2
#define PM_ALL_BY_SIZE "settings_storage sram_primary app"
#define PM_ADDRESS 0x0
#define PM_SIZE 0x7e000
#define PM_SRAM_ADDRESS 0x20000000
#define PM_SRAM_SIZE 0x20000
#endif /* PM_CONFIG_H__ */