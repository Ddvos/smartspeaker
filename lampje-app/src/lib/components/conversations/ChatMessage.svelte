<script lang="ts">
  import type { Message } from '$lib/types';
  import { Play } from 'lucide-svelte';
  import { formatRelativeTime } from '$lib/utils/formatters';

  let {
    message
  }: {
    message: Message;
  } = $props();
</script>

{#if message.role === 'user'}
  <div class="flex justify-end">
    <div class="max-w-[75%]">
      <div class="rounded-xl rounded-br-md bg-surface-2 px-4 py-3">
        <p class="font-body text-sm leading-relaxed text-on-bg">{message.content}</p>
      </div>
      <div class="mt-1.5 flex items-center justify-end gap-2">
        {#if message.audioUrl}
          <button
            type="button"
            class="cursor-pointer text-on-bg-muted transition-colors duration-150 hover:text-primary"
            aria-label="Audio afspelen"
          >
            <Play size={14} />
          </button>
        {/if}
        <span class="font-label text-[11px] text-on-bg-muted">{formatRelativeTime(message.timestamp)}</span>
      </div>
    </div>
  </div>
{:else}
  <div class="flex justify-start">
    <div class="max-w-[75%]">
      <div class="rounded-xl rounded-bl-md bg-surface-1 px-4 py-3">
        <p class="font-body text-sm leading-relaxed text-on-bg">{message.content}</p>
      </div>
      <div class="mt-1.5 flex items-center gap-2">
        {#if message.audioUrl}
          <button
            type="button"
            class="cursor-pointer text-on-bg-muted transition-colors duration-150 hover:text-primary"
            aria-label="Audio afspelen"
          >
            <Play size={14} />
          </button>
        {/if}
        <span class="font-label text-[11px] text-on-bg-muted">{formatRelativeTime(message.timestamp)}</span>
      </div>
    </div>
  </div>
{/if}
