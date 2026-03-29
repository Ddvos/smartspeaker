<script lang="ts">
  import { deviceStore } from '$lib/stores/devices.svelte';

  let digits = $state(['', '', '', '', '', '']);
  let loading = $state(false);
  let error = $state<string | null>(null);
  let success = $state(false);
  let inputs: HTMLInputElement[] = [];

  function handleInput(index: number, event: Event) {
    const target = event.target as HTMLInputElement;
    const value = target.value.replace(/\D/g, '');
    digits[index] = value.slice(-1);

    if (value && index < 5) {
      inputs[index + 1]?.focus();
    }

    // Auto-submit when all 6 digits are entered
    if (digits.every((d) => d !== '')) {
      submit();
    }
  }

  function handleKeydown(index: number, event: KeyboardEvent) {
    if (event.key === 'Backspace' && !digits[index] && index > 0) {
      inputs[index - 1]?.focus();
    }
  }

  function handlePaste(event: ClipboardEvent) {
    event.preventDefault();
    const text = event.clipboardData?.getData('text')?.replace(/\D/g, '') ?? '';
    if (text.length === 6) {
      for (let i = 0; i < 6; i++) {
        digits[i] = text[i];
      }
      inputs[5]?.focus();
      submit();
    }
  }

  async function submit() {
    const code = digits.join('');
    if (code.length !== 6) return;

    loading = true;
    error = null;

    const device = await deviceStore.pairDevice(code);
    loading = false;

    if (device) {
      success = true;
    } else {
      error = deviceStore.error;
      digits = ['', '', '', '', '', ''];
      inputs[0]?.focus();
    }
  }
</script>

<div class="rounded-xl bg-surface-2 p-6">
  {#if success}
    <div class="text-center">
      <div class="mx-auto mb-3 flex h-12 w-12 items-center justify-center rounded-full bg-green-500/10">
        <span class="text-2xl text-green-500">&#10003;</span>
      </div>
      <h2 class="font-display text-lg text-on-bg">Apparaat gekoppeld!</h2>
      <p class="mt-2 font-body text-sm text-on-bg-dim">
        Je Lampje is succesvol verbonden met je account.
      </p>
    </div>
  {:else}
    <h2 class="font-display text-lg text-on-bg">Koppel je Lampje</h2>
    <p class="mt-2 font-body text-sm text-on-bg-dim">
      Voer de 6-cijferige code in die op het scherm van je Lampje wordt getoond.
    </p>

    <div class="mt-6 flex justify-center gap-3" onpaste={handlePaste}>
      {#each digits as digit, i}
        <input
          bind:this={inputs[i]}
          type="text"
          inputmode="numeric"
          maxlength="1"
          class="h-14 w-11 rounded-xl bg-surface-0 text-center font-display text-2xl text-on-bg outline-none ring-2 ring-transparent transition-all focus:ring-primary/40"
          value={digit}
          oninput={(e) => handleInput(i, e)}
          onkeydown={(e) => handleKeydown(i, e)}
          disabled={loading}
        />
      {/each}
    </div>

    {#if error}
      <p class="mt-4 text-center font-body text-sm text-red-400">{error}</p>
    {/if}

    {#if loading}
      <p class="mt-4 text-center font-body text-sm text-on-bg-muted">Koppelen...</p>
    {/if}
  {/if}
</div>
