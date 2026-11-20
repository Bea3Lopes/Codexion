/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besoares <besoares@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/20 14:43:21 by besoares          #+#    #+#             */
/*   Updated: 2026/11/20 14:59:26 by besoares         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int coder_number(t_coder *coder, t_coder *coders)
{
    int i;

    i = 0;
    while (i < 5)
    {
        if (coder == &coders[i])
            return (i + 1);
        i++;
    }
    return (0);
}

int main(void)
{
    t_simulation    sim;
    t_config        config;
    t_heap          *heap;
    t_coder         coders[5];
    t_dongle        dongles[5];
    int             i;
    int             errors;

    /* Initialize simulation */
    config.time_to_burnout = 1000;
    sim.config = &config;
    sim.scheduler = EDF;
    sim.stop_flag = 0;

    pthread_mutex_init(&sim.resource_mutex, NULL);
    pthread_cond_init(&sim.resource_cond, NULL);

    heap = create_heap(5);
    if (!heap)
        return (1);
    sim.heap = heap;

    /* Initialize dongles */
    i = 0;
    while (i < 5)
    {
        dongles[i].available = 1;
        dongles[i].cooldown_until = 0;
        i++;
    }

    /* Make dongle 1 unavailable */
    dongles[0].available = 0;

    /* Initialize coders and their dongles */
    i = 0;
    while (i < 5)
    {
        coders[i].left_dongle = &dongles[i];
        coders[i].right_dongle = &dongles[(i + 1) % 5];
        coders[i].last_compile_start = (i + 1) * 100;
        coders[i].grant = 0;

        insert_heap(&sim, heap, &coders[i]);
        i++;
    }

    /* Check initial heap */
    printf("--- BEFORE DISPATCH ---\n");
    printf("Heap root: coder %d\n",
        coder_number(heap->coders_queue[0], coders));

    /* Run dispatcher */
    printf("\n--- DISPATCH ---\n");
    dispatch_request(&sim);

    /* Check results */
    printf("\n--- RESULTS ---\n");
    errors = 0;
    i = 0;
    while (i < 5)
    {
        printf("Coder %d: %s\n", i + 1,
            coders[i].grant ? "GRANTED" : "WAITING");

        if ((i == 1 || i == 3) && !coders[i].grant)
            errors++;
        if ((i != 1 && i != 3) && coders[i].grant)
            errors++;
        i++;
    }

    printf("\nRemaining requests in heap: %d\n", heap->size);

    if (errors == 0 && heap->size == 3)
        printf("ALL TESTS PASSED!\n");
    else
        printf("TEST FAILED!\n");

    /* Cleanup */
    free_heap(heap);
    pthread_cond_destroy(&sim.resource_cond);
    pthread_mutex_destroy(&sim.resource_mutex);

    return (errors != 0 || heap->size != 3);
}


