import time

class Timer:
    def __init__(self, epoch_size):
        self.epoch_size = epoch_size
        self.start_time = time.time()
        self.pre_time = time.time()
        self.epoch = 0
    
    def update(self):
        self.epoch += 1
        now_time = time.time()
        tot_time = now_time - self.start_time
        expext_time = tot_time * self.epoch_size / self.epoch
        remain_time = expext_time - tot_time
        epoch_time = now_time - self.pre_time
        self.pre_time = now_time
        print("epoch_time: %02d:%02d:%02d, expect_time: %02d:%02d:%02d, remain_time: %02d:%02d:%02d"%(
                epoch_time/3600, (epoch_time%3600)/60, epoch_time%60,
                expext_time/3600, (expext_time%3600)/60, expext_time%60,
                remain_time/3600, (remain_time%3600)/60, remain_time%60
            )
        )