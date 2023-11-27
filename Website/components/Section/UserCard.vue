<script setup>
  const user = useSupabaseUser();
  const supabase = useSupabaseClient();
  const logoutLoading = ref(false);
  const toast = useToast();

  async function logout() {
    logoutLoading.value = true;

    const { error } = await supabase.auth.signOut();

    if (error) {
      toast.add({ title: 'Error logging out! Please try again.' });
      logoutLoading.value = false;
      return;
    }

    toast.add({ title: 'Signed out! Comeback soon' });
    logoutLoading.value = false;
    return navigateTo('/login');
  }
</script>

<template>
  <section class="flex items-center gap-4">
    <UIcon name="i-solar-hand-shake-outline" class="text-5xl text-primary" />

    <div>
      <p class="app-caption">Welcome Back!</p>
      <h1 class="text-lg font-medium" v-if="user">{{ user.email }}</h1>
    </div>

    <UButton
      @click="logout"
      :loading="logoutLoading"
      variant="outline"
      size="xs"
    >
      Sign out
    </UButton>
  </section>
</template>
