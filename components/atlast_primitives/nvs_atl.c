#include "atldef.h"
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <errno.h>

#include "nvs.h"
#include "esp_log.h"

#include "common.h"

const char *TAG = "NVS Operations";

typedef struct
{
	nvs_type_t type;
	const char *str;
} type_str_pair_t;

static const type_str_pair_t type_str_pair[] = {
		{NVS_TYPE_I8, "i8"},
		{NVS_TYPE_U8, "u8"},
		{NVS_TYPE_U16, "u16"},
		{NVS_TYPE_I16, "i16"},
		{NVS_TYPE_U32, "u32"},
		{NVS_TYPE_I32, "i32"},
		{NVS_TYPE_U64, "u64"},
		{NVS_TYPE_I64, "i64"},
		{NVS_TYPE_STR, "str"},
		{NVS_TYPE_BLOB, "blob"},
		{NVS_TYPE_ANY, "any"},
};

static const size_t TYPE_STR_PAIR_SIZE = sizeof(type_str_pair) / sizeof(type_str_pair[0]);

static nvs_type_t str_to_type(const char *type)
{
	for (int i = 0; i < TYPE_STR_PAIR_SIZE; i++)
	{
		const type_str_pair_t *p = &type_str_pair[i];
		if (strcmp(type, p->str) == 0)
		{
			return p->type;
		}
	}

	return NVS_TYPE_ANY;
}

static const char *type_to_str(nvs_type_t type)
{
	for (int i = 0; i < TYPE_STR_PAIR_SIZE; i++)
	{
		const type_str_pair_t *p = &type_str_pair[i];
		if (p->type == type)
		{
			return p->str;
		}
	}

	return "Unknown";
}

// static esp_err_t erase(const char *key)
// {
//     nvs_handle_t nvs;

//     esp_err_t err = nvs_open(current_namespace, NVS_READWRITE, &nvs);
//     if (err == ESP_OK) {
//         err = nvs_erase_key(nvs, key);
//         if (err == ESP_OK) {
//             err = nvs_commit(nvs);
//             if (err == ESP_OK) {
//                 ESP_LOGI(TAG, "Value with key '%s' erased", key);
//             }
//         }
//         nvs_close(nvs);
//     }

//     return err;
// }

// static esp_err_t erase_all(const char *name)
// {
//     nvs_handle_t nvs;

//     esp_err_t err = nvs_open(name, NVS_READWRITE, &nvs);
//     if (err == ESP_OK) {
//         err = nvs_erase_all(nvs);
//         if (err == ESP_OK) {
//             err = nvs_commit(nvs);
//         }
//     }

//     ESP_LOGI(TAG, "Namespace '%s' was %s erased", name, (err == ESP_OK) ? "" : "not");

//     nvs_close(nvs);
//     return ESP_OK;
// }

static int list(const char *part, const char *name, const char *str_type)
{
	nvs_type_t type = str_to_type(str_type);

	nvs_iterator_t it = nvs_entry_find(part, NULL, type);
	if (it == NULL)
	{
		ESP_LOGE(TAG, "No such enty was found");
		return 1;
	}

	do
	{
		nvs_entry_info_t info;
		nvs_entry_info(it, &info);
		it = nvs_entry_next(it);

		printf("namespace '%s', key '%s', type '%s' \n",
					 info.namespace_name, info.key, type_to_str(info.type));
	} while (it != NULL);

	return 0;
}

prim P_nvs_get()
{ /* strength dest -- */
	Sl(3);
	Hpc(S0);
	Hpc(S1);
	Hpc(S2);

	const char *current_namespace = (const char *)S2;
	const char *str_type = (const char *)S1;
	const char *key = (const char *)S0;


	nvs_handle_t nvs;
	esp_err_t err;

	nvs_type_t type = str_to_type(str_type);

	if (type == NVS_TYPE_ANY)
	{
		ESP_LOGE(TAG, "Type '%s' is undefined", str_type);
		return ESP_ERR_NVS_TYPE_MISMATCH;
	}

	err = nvs_open(current_namespace, NVS_READONLY, &nvs);
	if (err != ESP_OK)
	{
		// return err;
	}

	if (type == NVS_TYPE_I8)
	{
		int8_t value;
		err = nvs_get_i8(nvs, key, &value);
		if (err == ESP_OK)
		{
			Push = value;
			printf("%d\n", value);
		}
	}
	else if (type == NVS_TYPE_U8)
	{
		uint8_t value;
		err = nvs_get_u8(nvs, key, &value);
		if (err == ESP_OK)
		{
			Push = value;
			printf("%u\n", value);
		}
	}
	else if (type == NVS_TYPE_I16)
	{
		int16_t value;
		err = nvs_get_i16(nvs, key, &value);
		if (err == ESP_OK)
		{
			Push = value;
			printf("%u\n", value);
		}
	}
	else if (type == NVS_TYPE_U16)
	{
		uint16_t value;
		if ((err = nvs_get_u16(nvs, key, &value)) == ESP_OK)
		{
			Push = value;
			printf("%u\n", value);
		}
	}
	else if (type == NVS_TYPE_I32)
	{
		int32_t value;
		if ((err = nvs_get_i32(nvs, key, &value)) == ESP_OK)
		{
			Push = value;
			printf("%d\n", value);
		}
	}
	else if (type == NVS_TYPE_U32)
	{
		uint32_t value;
		if ((err = nvs_get_u32(nvs, key, &value)) == ESP_OK)
		{
			Push = value;
			printf("%u\n", value);
		}
	}
	else if (type == NVS_TYPE_I64)
	{
		int64_t value;
		if ((err = nvs_get_i64(nvs, key, &value)) == ESP_OK)
		{
			Push = value;
			printf("%lld\n", value);
		}
	}
	else if (type == NVS_TYPE_U64)
	{
		uint64_t value;
		if ((err = nvs_get_u64(nvs, key, &value)) == ESP_OK)
		{
			Push = value;
			printf("%llu\n", value);
		}
	}
	else if (type == NVS_TYPE_STR)
	{
		size_t len;
		if ((err = nvs_get_str(nvs, key, NULL, &len)) == ESP_OK)
		{
			char *str = (char *)malloc(len);
			// Sl(1);
			// Ho((S0 + 1 + sizeof(stackitem)) / sizeof(stackitem));
			// P_create(); /* Create variable */
			// /* Allocate storage for string */
			// hptr += (S0 + 1 + sizeof(stackitem)) / sizeof(stackitem);
			if ((err = nvs_get_str(nvs, key, (char *)S0, &len)) == ESP_OK)
			{
				printf("%s\n", (char *)str);
			}
			free(str);
		}
	}
	// else if (type == NVS_TYPE_BLOB)
	// {
	// 	size_t len;
	// 	if ((err = nvs_get_blob(nvs, key, NULL, &len)) == ESP_OK)
	// 	{
	// 		char *blob = (char *)malloc(len);
	// 		if ((err = nvs_get_blob(nvs, key, blob, &len)) == ESP_OK)
	// 		{
	// 			print_blob(blob, len);
	// 		}
	// 		free(blob);
	// 	}
	// }
	// strcpy

	nvs_close(nvs);
	// return err;


}

prim P_nvs_set()
{
	Sl(4);
	Hpc(S0);
	Hpc(S1);
	Hpc(S2);
	Hpc(S3);

	const char *current_namespace = (const char *)S3;
	const char *str_type = (const char *)S2;
	const char *key = (const char *)S1;
	const char *str_value = (const char *)S0;

	// static esp_err_t set_value_in_nvs(const char *current_namespace, const char *key, const char *str_type, const char *str_value)
	// {
	esp_err_t err;
	nvs_handle_t nvs;
	bool range_error = false;

	nvs_type_t type = str_to_type(str_type);

	if (type == NVS_TYPE_ANY)
	{
		ESP_LOGE(TAG, "Type '%s' is undefined", str_type);
		// return ESP_ERR_NVS_TYPE_MISMATCH;
	}

	err = nvs_open(current_namespace, NVS_READWRITE, &nvs);
	if (err != ESP_OK)
	{
		// return err;
	}

	if (type == NVS_TYPE_I8)
	{
		int32_t value = strtol(str_value, NULL, 0);
		if (value < INT8_MIN || value > INT8_MAX || errno == ERANGE)
		{
			range_error = true;
		}
		else
		{
			err = nvs_set_i8(nvs, key, (int8_t)value);
		}
	}
	else if (type == NVS_TYPE_U8)
	{
		uint32_t value = strtoul(str_value, NULL, 0);
		if (value > UINT8_MAX || errno == ERANGE)
		{
			range_error = true;
		}
		else
		{
			err = nvs_set_u8(nvs, key, (uint8_t)value);
		}
	}
	else if (type == NVS_TYPE_I16)
	{
		int32_t value = strtol(str_value, NULL, 0);
		if (value < INT16_MIN || value > INT16_MAX || errno == ERANGE)
		{
			range_error = true;
		}
		else
		{
			err = nvs_set_i16(nvs, key, (int16_t)value);
		}
	}
	else if (type == NVS_TYPE_U16)
	{
		uint32_t value = strtoul(str_value, NULL, 0);
		if (value > UINT16_MAX || errno == ERANGE)
		{
			range_error = true;
		}
		else
		{
			err = nvs_set_u16(nvs, key, (uint16_t)value);
		}
	}
	else if (type == NVS_TYPE_I32)
	{
		int32_t value = strtol(str_value, NULL, 0);
		if (errno != ERANGE)
		{
			err = nvs_set_i32(nvs, key, value);
		}
	}
	else if (type == NVS_TYPE_U32)
	{
		uint32_t value = strtoul(str_value, NULL, 0);
		if (errno != ERANGE)
		{
			err = nvs_set_u32(nvs, key, value);
		}
	}
	else if (type == NVS_TYPE_I64)
	{
		int64_t value = strtoll(str_value, NULL, 0);
		if (errno != ERANGE)
		{
			err = nvs_set_i64(nvs, key, value);
		}
	}
	else if (type == NVS_TYPE_U64)
	{
		uint64_t value = strtoull(str_value, NULL, 0);
		if (errno != ERANGE)
		{
			err = nvs_set_u64(nvs, key, value);
		}
	}
	else if (type == NVS_TYPE_STR)
	{
		err = nvs_set_str(nvs, key, str_value);
	}
	// else if (type == NVS_TYPE_BLOB)
	// {
	// 	err = store_blob(nvs, key, str_value);
	// }

	if (range_error || errno == ERANGE)
	{
		nvs_close(nvs);
		// return ESP_ERR_NVS_VALUE_TOO_LONG;
	}

	if (err == ESP_OK)
	{
		err = nvs_commit(nvs);
		if (err == ESP_OK)
		{
			ESP_LOGI(TAG, "Value stored under key '%s'", key);
		}
	}

	nvs_close(nvs);
	// return err;
	Npop(4);
}

prim P_nvs_list()
{ /* strength dest -- */
	Sl(1);
	Hpc(S0);
	// Hpc(S1);
	// Hpc(S2);

	char *part = "nvs"; // (char *)S1;
	char *namespace = (char *)S0;
	// char *str_type = (char *)S0;

	// nvs_type_t type = str_to_type(str_type);

	nvs_iterator_t it = nvs_entry_find(part, namespace, NVS_TYPE_ANY);
	if (it == NULL)
	{
		ESP_LOGE(TAG, "No such enty was found");
		return 1;
	}

	do
	{
		nvs_entry_info_t info;
		nvs_entry_info(it, &info);
		it = nvs_entry_next(it);

		printf("namespace '%s', key '%s', type '%s' \n",
					 info.namespace_name, info.key, type_to_str(info.type));
	} while (it != NULL);
	
	Npop(2);
}

struct primfcn nvs_fcns[] = {
		{"0NVS.GET", P_nvs_get},
		{"0NVS.SET", P_nvs_set},
		{"0NVS.LIST", P_nvs_list},
		// {"0NVS.ERASE",	P_mnemonic_generate},
		// {"0NVS.ERASEALL",	P_mnemonic_generate},
		{NULL, (codeptr)0}};
