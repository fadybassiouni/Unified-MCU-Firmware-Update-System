<script setup>
  // Reading socketHost initial value from runtime config
  const runtimeConfig = useRuntimeConfig().public;
  const carToken = ref(runtimeConfig.carToken || '');
  const emit = defineEmits(['connect', 'disconnect']);
  function emitConnect() {
    emit('connect', carToken.value);
  }

  // Car Status
  const { carConnection } = useStatus();
  const carStatusClass = computed(() => ({
    'text-red-500': carConnection.value === 0,
    'text-green-500': carConnection.value === 1,
    'text-orange-500': carConnection.value === 2,
  }));
</script>

<template>
  <AppCard class="gap-12">
    <div class="w-full gap-2 flex flex-col" v-auto-animate>
      <div class="flex justify-between items-center">
        <div class="flex items-center gap-2">
          <StatusDot :class="carStatusClass" />
          <p>Car</p>
        </div>

        <UButton
          v-if="carConnection === 1"
          variant="outline"
          @click="$emit('disconnect')"
        >
          Disconnect
        </UButton>
      </div>

      <!-- Entering Car Token and Connecting to it -->
      <div class="flex gap-4" v-if="carConnection !== 1">
        <UInput
          @keyup.self.enter="emitConnect"
          :disabled="carConnection === 2"
          :ui="{ icon: { trailing: { pointer: '' } } }"
          v-model="carToken"
          placeholder="Car token"
          class="w-full"
        >
          <template #trailing>
            <UButton
              @click="carToken = ''"
              :disabled="carConnection === 2"
              :padded="false"
              v-show="carToken !== ''"
              color="gray"
              variant="link"
              icon="i-heroicons-x-mark-20-solid"
            />
          </template>
        </UInput>

        <UButton :loading="carConnection === 2" @click="emitConnect">
          {{ carConnection === 2 ? 'Connecting' : 'Connect' }}
        </UButton>
      </div>
    </div>
  </AppCard>
</template>
