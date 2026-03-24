<script lang="ts">
  import Toast from './Toast.svelte';
  import { toastStore } from '$lib/stores/toast.svelte';
</script>

<div class="fixed top-4 right-4 z-50 flex flex-col gap-2 w-80">
  {#each toastStore.items as toast (toast.id)}
    <Toast
      message={toast.message}
      type={toast.type}
      ondismiss={() => toastStore.dismiss(toast.id)}
    />
  {/each}
</div>
