/**
* @brief 
* @file web3_shell.c
* @author J.H. 
* @date 2018-10-19
*/

/* system includes */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <zephyr.h>
#include <shell/shell.h>
#pragma GCC diagnostic pop

/* local includes */
#include "web3_shell.h"
#include "eth/web3.h"
#include "helpers/hextobin.h"
#include "web3_json.h"
#include "utils.h"
#include "http_service.h"

#include "web3_rpc.h"

static int web3_shell_blockNumber(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(shell);
    ARG_UNUSED(argv);
    ARG_UNUSED(argc);

    uint64_t blk_num = 0;
    web3_eth_blockNumber(&blk_num);
    printk("%lld\n", blk_num);

    return 0;
}

static int web3_shell_getBalance(const struct shell *shell, size_t argc, char *argv[])
{
    if(argc < 1) {
        printk("missing argument: address\n");
        return 0;
    }
    address_t address = {0};
    if(hextobin(argv[1], address.a, sizeof(address.a)) < 0) {
        printk("invalid argument: address\n");
        return 0;
    }
    uint256_t out;
    web3_eth_getBalance(&address, &out);
    printk_uint256(shell, &out);
    return 0;
}

static int web3_shell_getTransactionCount(const struct shell *shell, size_t argc, char *argv[])
{
    if(argc < 1) {
        printk("missing argument: address\n");
        return 0;
    }
    address_t address = {0};
    if(hextobin(argv[1], address.a, sizeof(address.a)) < 0) {
        printk("invalid argument: address\n");
        return 0;
    }
    uint256_t out;
    web3_eth_getTransactionCount(&address, &out);
    printk_uint256(shell, &out);
    return 0;
}

static int web3_shell_sendRawTransaction(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(argv);
    ARG_UNUSED(argc);

    uint8_t databuf[128];
    if(argc < 1) {
        printk("missing argument: data\n");
        return 0;
    }
    int data_len = hextobin(argv[1], databuf, sizeof(databuf));
    if(data_len < 0) {
        printk("invalid argument: data\n");
        return 0;
    }
    uint256_t tx_hash;
    if(web3_eth_sendRawTransaction(databuf, data_len, &tx_hash) < 0) {
        printk("eth_sendRawTransaction failed\n");
        return 0;
    }
    printk_uint256(shell, &tx_hash);

    return 0;
}

SHELL_CREATE_STATIC_SUBCMD_SET(sub_web3) {
    SHELL_CMD(eth_getTransactionCount, NULL, "", web3_shell_getTransactionCount),
    SHELL_CMD(eth_getBalance, NULL, "", web3_shell_getBalance),
    SHELL_CMD(eth_blockNumber, NULL, "", web3_shell_blockNumber),
    SHELL_CMD(eth_sendRawTransaction, NULL, "", web3_shell_sendRawTransaction),
	SHELL_SUBCMD_SET_END
};
SHELL_CMD_REGISTER(web3, &sub_web3, "http upload service", NULL);
