; ModuleID = 'unpeeled.c'
source_filename = "unpeeled.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.nodeOneOld = type { i32, double, i32, [4 x i8], [6 x i8], i8, [67 x double], double, double, double, float }

@arrayOneOld = dso_local global [1000 x %struct.nodeOneOld] zeroinitializer, align 16
@arrayTwoOld = dso_local global [1000 x %struct.nodeOneOld] zeroinitializer, align 16
@.str = private unnamed_addr constant [16 x i8] c"Validity check\0A\00", align 1
@.str.1 = private unnamed_addr constant [11 x i8] c"\0Aa: ---\0A%d\00", align 1
@.str.2 = private unnamed_addr constant [11 x i8] c"\0Ab: ---\0A%f\00", align 1
@.str.3 = private unnamed_addr constant [11 x i8] c"\0Ac: ---\0A%d\00", align 1
@.str.4 = private unnamed_addr constant [13 x i8] c"\0Ad: ---\0A%lld\00", align 1
@.str.5 = private unnamed_addr constant [11 x i8] c"\0Ae: ---\0A%d\00", align 1
@.str.6 = private unnamed_addr constant [11 x i8] c"\0Ag: ---\0A%f\00", align 1
@.str.7 = private unnamed_addr constant [11 x i8] c"\0Ah: ---\0A%f\00", align 1
@.str.8 = private unnamed_addr constant [11 x i8] c"\0Ai: ---\0A%f\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeOneOld(i32 noundef %size) #0 {
entry:
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc27, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end29

for.body:                                         ; preds = %for.cond
  %2 = load i32, ptr %i, align 4
  %idxprom = sext i32 %2 to i64
  %arrayidx = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 0
  store i32 1, ptr %a, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %3 to i64
  %arrayidx2 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom1
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx2, i32 0, i32 1
  store double 1.000000e+01, ptr %b, align 8
  %4 = load i32, ptr %i, align 4
  %idxprom3 = sext i32 %4 to i64
  %arrayidx4 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom3
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx4, i32 0, i32 2
  store i32 9, ptr %c, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom5 = sext i32 %5 to i64
  %arrayidx6 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom5
  %d = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx6, i32 0, i32 4
  store i48 23, ptr %d, align 8
  %6 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %6 to i64
  %arrayidx8 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom7
  %e = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 5
  store i8 97, ptr %e, align 2
  store i32 0, ptr %j, align 4
  br label %for.cond9

for.cond9:                                        ; preds = %for.inc, %for.body
  %7 = load i32, ptr %j, align 4
  %cmp10 = icmp slt i32 %7, 67
  br i1 %cmp10, label %for.body11, label %for.end

for.body11:                                       ; preds = %for.cond9
  %8 = load i32, ptr %i, align 4
  %idxprom12 = sext i32 %8 to i64
  %arrayidx13 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom12
  %f = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx13, i32 0, i32 6
  %9 = load i32, ptr %j, align 4
  %idxprom14 = sext i32 %9 to i64
  %arrayidx15 = getelementptr inbounds [67 x double], ptr %f, i64 0, i64 %idxprom14
  store double 2.300000e+01, ptr %arrayidx15, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body11
  %10 = load i32, ptr %j, align 4
  %inc = add nsw i32 %10, 1
  store i32 %inc, ptr %j, align 4
  br label %for.cond9, !llvm.loop !4

for.end:                                          ; preds = %for.cond9
  %11 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %11 to i64
  %arrayidx17 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom16
  %g = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx17, i32 0, i32 7
  store double 2.300000e+01, ptr %g, align 8
  %12 = load i32, ptr %i, align 4
  %idxprom18 = sext i32 %12 to i64
  %arrayidx19 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayTwoOld, i64 0, i64 %idxprom18
  %a20 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx19, i32 0, i32 0
  store i32 1, ptr %a20, align 8
  %13 = load i32, ptr %i, align 4
  %idxprom21 = sext i32 %13 to i64
  %arrayidx22 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayTwoOld, i64 0, i64 %idxprom21
  %b23 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx22, i32 0, i32 1
  store double 1.000000e+01, ptr %b23, align 8
  %14 = load i32, ptr %i, align 4
  %idxprom24 = sext i32 %14 to i64
  %arrayidx25 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayTwoOld, i64 0, i64 %idxprom24
  %c26 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx25, i32 0, i32 2
  store i32 9, ptr %c26, align 8
  br label %for.inc27

for.inc27:                                        ; preds = %for.end
  %15 = load i32, ptr %i, align 4
  %inc28 = add nsw i32 %15, 1
  store i32 %inc28, ptr %i, align 4
  br label %for.cond, !llvm.loop !6

for.end29:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multNodeOneOld(i32 noundef %size) #0 {
entry:
  %size.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc19, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 100000
  br i1 %cmp, label %for.body, label %for.end21

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %size.addr, align 4
  %cmp2 = icmp slt i32 %1, %2
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 1
  %4 = load double, ptr %b, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %5 to i64
  %arrayidx5 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom4
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx5, i32 0, i32 0
  %6 = load i32, ptr %a, align 8
  %conv = sitofp i32 %6 to double
  %7 = call double @llvm.fmuladd.f64(double %4, double %conv, double 2.000000e+01)
  %conv6 = fptosi double %7 to i32
  %8 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %8 to i64
  %arrayidx8 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom7
  %a9 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 0
  store i32 %conv6, ptr %a9, align 8
  %9 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %9 to i64
  %arrayidx11 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom10
  %a12 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx11, i32 0, i32 0
  %10 = load i32, ptr %a12, align 8
  %11 = load i32, ptr %i, align 4
  %idxprom13 = sext i32 %11 to i64
  %arrayidx14 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom13
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx14, i32 0, i32 2
  %12 = load i32, ptr %c, align 8
  %div = sdiv i32 %10, %12
  %sub = sub nsw i32 %div, 297
  %conv15 = sitofp i32 %sub to double
  %13 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %13 to i64
  %arrayidx17 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom16
  %b18 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx17, i32 0, i32 1
  store double %conv15, ptr %b18, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %14 = load i32, ptr %i, align 4
  %inc = add nsw i32 %14, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !7

for.end:                                          ; preds = %for.cond1
  br label %for.inc19

for.inc19:                                        ; preds = %for.end
  %15 = load i32, ptr %j, align 4
  %inc20 = add nsw i32 %15, 1
  store i32 %inc20, ptr %j, align 4
  br label %for.cond, !llvm.loop !8

for.end21:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare double @llvm.fmuladd.f64(double, double, double) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multArrays(i32 noundef %size) #0 {
entry:
  %size.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc18, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 100000
  br i1 %cmp, label %for.body, label %for.end20

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %size.addr, align 4
  %cmp2 = icmp slt i32 %1, %2
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 0
  %4 = load i32, ptr %a, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %5 to i64
  %arrayidx5 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayTwoOld, i64 0, i64 %idxprom4
  %a6 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx5, i32 0, i32 0
  %6 = load i32, ptr %a6, align 8
  %mul = mul nsw i32 %4, %6
  %7 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %7 to i64
  %arrayidx8 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom7
  %a9 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 0
  store i32 %mul, ptr %a9, align 8
  %8 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %8 to i64
  %arrayidx11 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom10
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx11, i32 0, i32 1
  %9 = load double, ptr %b, align 8
  %10 = load i32, ptr %i, align 4
  %idxprom12 = sext i32 %10 to i64
  %arrayidx13 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayTwoOld, i64 0, i64 %idxprom12
  %a14 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx13, i32 0, i32 0
  %11 = load i32, ptr %a14, align 8
  %conv = sitofp i32 %11 to double
  %add = fadd double %9, %conv
  %div = fdiv double %add, 1.100000e+00
  %12 = load i32, ptr %i, align 4
  %idxprom15 = sext i32 %12 to i64
  %arrayidx16 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom15
  %b17 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx16, i32 0, i32 1
  store double %div, ptr %b17, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %13 = load i32, ptr %i, align 4
  %inc = add nsw i32 %13, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !9

for.end:                                          ; preds = %for.cond1
  br label %for.inc18

for.inc18:                                        ; preds = %for.end
  %14 = load i32, ptr %j, align 4
  %inc19 = add nsw i32 %14, 1
  store i32 %inc19, ptr %j, align 4
  br label %for.cond, !llvm.loop !10

for.end20:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 noundef %argc, ptr noundef %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca ptr, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  store i32 %argc, ptr %argc.addr, align 4
  store ptr %argv, ptr %argv.addr, align 8
  call void @populateNodeOneOld(i32 noundef 1000)
  %call = call i32 @rand() #4
  %rem = srem i32 %call, 1000
  store i32 %rem, ptr %i, align 4
  %call1 = call i32 @rand() #4
  %rem2 = srem i32 %call1, 67
  store i32 %rem2, ptr %j, align 4
  %0 = load i32, ptr %i, align 4
  %idxprom = sext i32 %0 to i64
  %arrayidx = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 0
  store i32 1, ptr %a, align 8
  %1 = load i32, ptr %i, align 4
  %idxprom3 = sext i32 %1 to i64
  %arrayidx4 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom3
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx4, i32 0, i32 1
  store double 1.000000e+01, ptr %b, align 8
  %2 = load i32, ptr %i, align 4
  %idxprom5 = sext i32 %2 to i64
  %arrayidx6 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom5
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx6, i32 0, i32 2
  store i32 9, ptr %c, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %3 to i64
  %arrayidx8 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayTwoOld, i64 0, i64 %idxprom7
  %a9 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 0
  store i32 1, ptr %a9, align 8
  %4 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %4 to i64
  %arrayidx11 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayTwoOld, i64 0, i64 %idxprom10
  %b12 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx11, i32 0, i32 1
  store double 1.000000e+01, ptr %b12, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom13 = sext i32 %5 to i64
  %arrayidx14 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayTwoOld, i64 0, i64 %idxprom13
  %c15 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx14, i32 0, i32 2
  store i32 9, ptr %c15, align 8
  call void @multNodeOneOld(i32 noundef 1000)
  call void @multArrays(i32 noundef 1000)
  %call16 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %6 = load i32, ptr %i, align 4
  %idxprom17 = sext i32 %6 to i64
  %arrayidx18 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom17
  %a19 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx18, i32 0, i32 0
  %7 = load i32, ptr %a19, align 8
  %call20 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %7)
  %8 = load i32, ptr %i, align 4
  %idxprom21 = sext i32 %8 to i64
  %arrayidx22 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom21
  %b23 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx22, i32 0, i32 1
  %9 = load double, ptr %b23, align 8
  %call24 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, double noundef %9)
  %10 = load i32, ptr %i, align 4
  %idxprom25 = sext i32 %10 to i64
  %arrayidx26 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom25
  %c27 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx26, i32 0, i32 2
  %11 = load i32, ptr %c27, align 8
  %call28 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %11)
  %12 = load i32, ptr %i, align 4
  %idxprom29 = sext i32 %12 to i64
  %arrayidx30 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom29
  %d = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx30, i32 0, i32 4
  %bf.load = load i48, ptr %d, align 8
  %bf.cast = sext i48 %bf.load to i64
  %call31 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, i64 noundef %bf.cast)
  %13 = load i32, ptr %i, align 4
  %idxprom32 = sext i32 %13 to i64
  %arrayidx33 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom32
  %e = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx33, i32 0, i32 5
  %14 = load i8, ptr %e, align 2
  %conv = sext i8 %14 to i32
  %call34 = call i32 (ptr, ...) @printf(ptr noundef @.str.5, i32 noundef %conv)
  %15 = load i32, ptr %i, align 4
  %idxprom35 = sext i32 %15 to i64
  %arrayidx36 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom35
  %g = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx36, i32 0, i32 7
  %16 = load double, ptr %g, align 8
  %call37 = call i32 (ptr, ...) @printf(ptr noundef @.str.6, double noundef %16)
  %17 = load i32, ptr %i, align 4
  %idxprom38 = sext i32 %17 to i64
  %arrayidx39 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom38
  %h = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx39, i32 0, i32 8
  %18 = load double, ptr %h, align 8
  %call40 = call i32 (ptr, ...) @printf(ptr noundef @.str.7, double noundef %18)
  %19 = load i32, ptr %i, align 4
  %idxprom41 = sext i32 %19 to i64
  %arrayidx42 = getelementptr inbounds [1000 x %struct.nodeOneOld], ptr @arrayOneOld, i64 0, i64 %idxprom41
  %i43 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx42, i32 0, i32 9
  %20 = load double, ptr %i43, align 8
  %call44 = call i32 (ptr, ...) @printf(ptr noundef @.str.8, double noundef %20)
  ret i32 0
}

; Function Attrs: nounwind
declare dso_local i32 @rand() #2

declare dso_local i32 @printf(ptr noundef, ...) #3

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 16.0.6 (Red Hat 16.0.6-2.module+el8.9.0+1651+e10a8f6d)"}
!4 = distinct !{!4, !5}
!5 = !{!"llvm.loop.mustprogress"}
!6 = distinct !{!6, !5}
!7 = distinct !{!7, !5}
!8 = distinct !{!8, !5}
!9 = distinct !{!9, !5}
!10 = distinct !{!10, !5}
