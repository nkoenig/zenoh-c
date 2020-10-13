/*
 * Copyright (c) 2017, 2020 ADLINK Technology Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Apache License, Version 2.0
 * which is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
 *
 * Contributors:
 *   ADLINK zenoh team, <zenoh@adlink-labs.tech>
 */
#include <stdio.h>
#include <string.h>
#include "zenoh/net.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("USAGE:\n\tzn_pub_thr <payload-size> [<zenoh-locator>]\n\n");
    exit(-1);
  }
  size_t len = atoi(argv[1]);  
  printf("Running throughput test for payload of %zu bytes.\n", len);
  ZNProperties *config = zn_config_peer();
  if (argc > 2) {
    zn_properties_add(config, ZN_PEER_KEY, argv[2]);
  }

  ZNSession *s = zn_open(config);
  if (s == 0) {
    printf("Unable to open session!\n");
    exit(-1);
  }

  char *data = (char*) malloc(len);
  memset(data, 1, len);

  ZNResKey *rid = zn_rid(zn_declare_resource(s, zn_rname("/test/thr")));
  ZNPublisher *pub = zn_declare_publisher(s, rid);
  if (pub == 0) {
      printf("Unable to declare publisher.\n");
      exit(-1);
  }

  while (1) {
    zn_write(s, rid, data, len);
  }
}