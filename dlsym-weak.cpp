/*
 * Copyright (C) 2008, 2009 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <android/log.h>
#include "linker.h"

static Elf_Sym* soinfo_elf_lookup(soinfo* si, unsigned hash, const char* name) {
    Elf_Sym* symtab = si->symtab;
    const char* strtab = si->strtab;

    for (unsigned n = si->bucket[hash % si->nbucket]; n != 0; n = si->chain[n]) {
        Elf_Sym* s = symtab + n;
        if (strcmp(strtab + s->st_name, name)) continue;

            /* only concern ourselves with global and weak symbol definitions */
        switch (ELF_ST_BIND(s->st_info)) {
        case STB_GLOBAL:
        case STB_WEAK:
            if (s->st_shndx == SHN_UNDEF) {
                continue;
            }
            return s;
        }
    }

    return NULL;
}

static unsigned elfhash(const char* _name) {
    const unsigned char* name = (const unsigned char*) _name;
    unsigned h = 0, g;

    while(*name) {
        h = (h << 4) + *name++;
        g = h & 0xf0000000;
        h ^= g;
        h ^= g >> 24;
    }
    return h;
}

/* This is used by dlsym(3).  It performs symbol lookup only within the
   specified soinfo object and not in any of its dependencies.

   TODO: Only looking in the specified soinfo seems wrong. dlsym(3) says
   that it should do a breadth first search through the dependency
   tree. This agrees with the ELF spec (aka System V Application
   Binary Interface) where in Chapter 5 it discuss resolving "Shared
   Object Dependencies" in breadth first search order.
 */
Elf_Sym* dlsym_handle_lookup(soinfo* si, const char* name) {
    return soinfo_elf_lookup(si, elfhash(name), name);
}

void* dlsym_weak(void* handle, const char* symbol) {

  soinfo* found = NULL;
  Elf_Sym* sym = NULL;
  found = reinterpret_cast<soinfo*>(handle);
  sym = dlsym_handle_lookup(found, symbol);

  if (sym != NULL) {
    return reinterpret_cast<void*>(sym->st_value + found->base/*load_bias*/);
  }
  __android_log_print(ANDROID_LOG_ERROR, "dlsym-weak", "Failed when looking up %s\n", symbol);
  return NULL;
}
