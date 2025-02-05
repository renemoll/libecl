# Bus scheduler design

For busses: SPI, I2C, CAN, RS485.

1. blocking transactions
   > waits until a transactions is complete
1. schedule transactions
   > queues the transactions and generates a callback when complete

Will use some bus driver (BusInterface -> read/write)
BusInterface will call BuScheduler when a transaction completed (success or failed)
