<script setup>
  // Checking if server is connected
  const { serverConnection } = useStatus();

  // Sending messages to the server
  const { sendMessage } = useSocketClient();
  const message = ref('');
  function sendNewMessage() {
    sendMessage(message.value);
    message.value = '';
  }

  // Rendering logs (last on top)
  const { logs } = useLogs();
  const reversedLogs = computed(() => logs.value.toReversed());

  function calculateTypeIconName(type) {
    switch (type) {
      case 'info':
        return 'i-simple-line-icons-info';
      case 'success':
        return 'i-simple-line-icons-check';
      case 'error':
        return 'i-simple-line-icons-close';
      case 'warning':
        return 'i-simple-line-icons-exclamation';
      case 'sent':
        return 'i-simple-line-icons-arrow-up-circle';
      case 'received':
        return 'i-simple-line-icons-arrow-down-circle';
      default:
        return 'i-simple-line-icons-info';
    }
  }

  function calculateTypeIconClass(type) {
    switch (type) {
      case 'info':
        return 'text-primary';
      case 'success':
        return 'text-green-500';
      case 'error':
        return 'text-red-500';
      case 'warning':
        return 'text-orange-500';
      case 'sent':
        return 'text-blue-500';
      case 'received':
        return 'text-blue-500';
      default:
        return 'text-primary';
    }
  }
</script>

<template>
  <section>
    <div class="app-heading mb-2">
      <UIcon name="i-teenyicons-terminal-outline" class="text-lg" />
      <h1>Logs</h1>
    </div>

    <AppCard
      v-auto-animate
      class="flex-col !items-start !py-2 text-sm gap-2 overflow-y-auto max-h-[12rem]"
    >
      <!-- Message Input -->
      <div class="flex gap-2 w-full" v-if="serverConnection === 1">
        <UInput
          @keyup.enter="sendNewMessage"
          :ui="{ icon: { trailing: { pointer: '' } } }"
          v-model="message"
          placeholder="Enter a message"
          class="w-full"
        >
          <template #trailing>
            <UButton
              @click="message = ''"
              :padded="false"
              v-show="message !== ''"
              color="gray"
              variant="link"
              icon="i-heroicons-x-mark-20-solid"
            />
          </template>
        </UInput>

        <UButton @click="sendNewMessage">Send</UButton>
      </div>

      <div v-if="!reversedLogs.length" class="flex items-center gap-2">
        <UIcon name="i-simple-line-icons-info" class="text-primary" />
        <p>Nothing yet.</p>
      </div>

      <!-- Messages -->
      <div
        v-for="item in reversedLogs"
        :key="item.id"
        class="flex items-center gap-2"
      >
        <UIcon
          :name="calculateTypeIconName(item.type)"
          :class="calculateTypeIconClass(item.type)"
        />

        <p>{{ item.message }}</p>
      </div>
    </AppCard>
  </section>
</template>
