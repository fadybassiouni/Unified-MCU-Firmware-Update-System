<script setup>
  useHead({ title: 'iTi MCU' });
</script>

<template>
  <div class="flex flex-col min-h-screen font-primary">
    <AppHeader />

    <main class="grow flex flex-col">
      <NuxtPage class="grow" />
    </main>

    <AppFooter />
    <UNotifications />
  </div>
</template>

<style>
  .app-text-secondary {
    @apply text-[#535353] dark:text-[#969E9E];
  }

  .app-caption {
    @apply text-xs font-medium text-[#535353] dark:text-[#969E9E];
  }

  .app-heading {
    @apply flex items-center gap-2 text-xl font-medium;
  }
</style>
