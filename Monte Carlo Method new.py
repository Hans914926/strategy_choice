import random
max_continueplay = 1
levels = [level for level in range(1, 4)]
probs = [1/len(levels) for level in levels]
mylevel = 3
states = [(partner_level,againcount)for partner_level in levels for againcount in range(0,max_continueplay+1)]
actions = ["continue","switch"]
s = [(0,0)]*1000
a = ["no choose"]*1000
Q={}
Q = {(state, action): 0 for state in states for action in actions} 
for simulation_time in range(1,1001):
    for partner_level1 in levels:
        state = (partner_level1, 0)
        for action in actions:
            G = 0
            s[0] = state
            a[0] = action
            for n in range(1,max_continueplay+2):
                partner_level,againcount = s[n-1]
                if(againcount == max_continueplay):  
                    a[n-1] = "switch"
                if(0 < againcount and againcount < max_continueplay):
                    a[n-1] = "continue"

                if(a[n-1] == "switch"):    
                    random_levels = random.choices(levels, weights=probs, k = 3)
                    s[n] = (random_levels[0],0)
                    if(mylevel+random_levels[0] > random_levels[1]+random_levels[2]):
                        G+=1
                    elif(mylevel+random_levels[0] == random_levels[1]+random_levels[2]):
                        G+=random.random()>0.5
                    else:
                        G+=0
                if(a[n-1] == "continue"):
                    random_levels = random.choices(levels, weights=probs, k = 2)
                    s[n] = (partner_level,againcount+1)
                    if(mylevel+partner_level > random_levels[0]+random_levels[1]):
                        G+=1
                    elif(mylevel+partner_level == random_levels[0]+random_levels[1]):
                        G+=random.random()>0.5
                    else:
                        G+=0
                
                max_Q = max(Q[(s[n], action)] for action in actions)
                best_action = [action for action in actions if Q[(s[n], action)] == max_Q]
                a[n] = random.choice(best_action)

            Q[(state,action)] = Q[(state,action)]+float(G-Q[(state,action)])/simulation_time

for state in states:
    partner_level,againcount = state
    if(againcount == 0):
        for action in actions:
            print(state,action,Q[((state,action))])

