<script setup>
  // Reading socketHost initial value from runtime config
  const runtimeConfig = useRuntimeConfig().public;
  const socketHost = ref(runtimeConfig.socketHost || '');
  const emit = defineEmits(['connect', 'disconnect']);
  function emitConnect() {
    emit('connect', socketHost.value);
  }

  // Server Status
  const { serverConnection } = useStatus();
  const serverStatusClass = computed(() => ({
    'text-red-500': serverConnection.value === 0,
    'text-green-500': serverConnection.value === 1,
    'text-orange-500': serverConnection.value === 2,
  }));
</script>

<template>
  <AppCard class="gap-12">
    <div class="w-full gap-2 flex flex-col" v-auto-animate>
      <div class="flex justify-between items-center">
        <div class="flex items-center gap-2">
          <StatusDot :class="serverStatusClass" />
          <p>Server</p>
        </div>

        <UButton
          v-if="serverConnection === 1"
          variant="outline"
          @click="$emit('disconnect')"
        >
          Disconnect
        </UButton>
      </div>

      <!-- Entering Server URL and Connecting to it -->
      <div class="flex gap-4" v-if="serverConnection !== 1">
        <UInput
          @keyup.self.enter="emitConnect"
          :disabled="serverConnection === 2"
          :ui="{ icon: { trailing: { pointer: '' } } }"
          v-model="socketHost"
          placeholder="Socket Server URL"
          class="w-full"
        >
          <template #trailing>
            <UButton
              @click="socketHost = ''"
              :disabled="serverConnection === 2"
              :padded="false"
              v-show="socketHost !== ''"
              color="gray"
              variant="link"
              icon="i-heroicons-x-mark-20-solid"
            />
          </template>
        </UInput>

        <UButton :loading="serverConnection === 2" @click="emitConnect">
          {{ serverConnection === 2 ? 'Connecting' : 'Connect' }}
        </UButton>
      </div>
    </div>
  </AppCard>
</template>
